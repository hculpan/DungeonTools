#include "db.h"

#include <QDebug>
#include <QDir>
#include <QString>
#include <QSqlRecord>

Db *db;

Db::Db(QFileInfo fileInfo, QObject *parent) : QObject(parent)
{
    this->dbFile = fileInfo;
    if (!dbFile.exists()) {
        if (!createNewDb()) {
            qInfo() << "Unable to create db: " << db.lastError();
        }
    } else {
        if (!initializeDb()) {
            qInfo() << "Unable to open db: " << db.lastError();
        }
    }
}

Db::~Db()
{
    db.close();
}

bool Db::updatePartyData(PartyData *partyData)
{
    m_lastError = "";
    PartyData *temp = fetchPartyData();
    QSqlQuery query;
    if (temp) {
        delete temp;
        query.prepare("update parties set size = ?, averageLevel = ? ");
    } else {
        query.prepare("insert into parties (size, averageLevel) values(?, ?)");
    }
    query.addBindValue(partyData->size);
    query.addBindValue(partyData->averageLevel);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

PartyData *Db::fetchPartyData()
{
    m_lastError = "";
    QSqlQuery query("select size, averageLevel from parties");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return nullptr;
    } else {
        if (query.first()) {
            return new PartyData(query.value(0).toInt(), query.value(1).toInt());
        } else {
            return nullptr;
        }
    }
}

bool Db::addEncounter(EncounterModel *encounter)
{
    m_lastError = "";
    QSqlQuery query("insert into encounters"
                    "(totalXp, monsterCount, partySize, averageLevel, difficulty, description)"
                    "values (?, ?, ?, ?, ?, ?)");
    query.addBindValue(encounter->xpTotal());
    query.addBindValue(encounter->monsterCount());
    query.addBindValue(encounter->partySize());
    query.addBindValue(encounter->averageLevel());
    query.addBindValue(encounter->difficulty);
    query.addBindValue(encounter->description);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    int encounterId = query.lastInsertId().toInt();
    for(MonsterGroup *m : encounter->monsterGroups()) {
        if (!addMonsterGroup(encounterId, m)) {
            return false;
        }
    }

    return true;
}

bool Db::addMonsterGroup(int encounterId, MonsterGroup *m)
{
    m_lastError = "";
    QSqlQuery query("insert into monsterGroups"
                    "(encounterId, name, cr, number, xp, hp, attackBonus, damage, saveDc, armorClass)"
                    "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(encounterId);
    query.addBindValue(m->name);
    query.addBindValue(m->cr);
    query.addBindValue(m->number);
    query.addBindValue(m->xp);
    query.addBindValue(m->hp);
    query.addBindValue(m->attackBonus);
    query.addBindValue(m->damage);
    query.addBindValue(m->saveDc);
    query.addBindValue(m->armorClass);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QList<EncounterInfo *> Db::fetchNonStartedEncounters()
{
    QList<EncounterInfo *> result;
    m_lastError = "";
    QSqlQuery query("select id, started, description from encounters where started = 0 order by id");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
    } else {
        while (query.next()) {
            EncounterInfo *e = new EncounterInfo(query.value(2).toString(), query.value(0).toInt(), query.value(1).toInt());
            result.append(e);
        }
    }
    return result;
}

QList<EncounterInfo *> Db::fetchAllEncounters()
{
    QList<EncounterInfo *> result;
    m_lastError = "";
    QSqlQuery query("select id, started, description from encounters order by id");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
    } else {
        while (query.next()) {
            EncounterInfo *e = new EncounterInfo(query.value(2).toString(), query.value(0).toInt(), query.value(1).toInt());
            result.append(e);
        }
    }
    return result;
}

Encounter *Db::fetchEncounter(int id)
{
    Encounter *result = nullptr;
    RecordSet e = fetchRecordById("encounters", id);
    if (e.size() > 0) {
        result = new Encounter();
        RecordSetIterator i(e);
        while (i.hasNext()) {
            i.next();
            result->setField(i.key(), &i.value());
        }

        QList<int> groupIds = fetchRecordsByForeignKey("monsterGroups", "encounterId", result->id);
        for (int j : groupIds) {
            MonsterGroup *m = fetchMonsterGroup(j);
            if (m) {
                result->groups.append(m);

                QList<int> monsterIds = fetchRecordsByForeignKey("monsters", "groupId", m->id);
                for (int k: monsterIds) {
                    Monster *mon = fetchMonster(k);
                    if (mon) {
                        m->monsters.append(mon);
                    }
                }
            }
        }
    }
    return result;
}

MonsterGroup *Db::fetchMonsterGroup(int id)
{
    MonsterGroup *result = nullptr;
    RecordSet e = fetchRecordById("monsterGroups", id);
    if (e.size() > 0) {
        result = new MonsterGroup();
        QMapIterator<QString, QVariant> i(e);
        while (i.hasNext()) {
            i.next();
            result->setField(i.key(), &(i.value()));
        }
    }
    return result;
}

Monster *Db::fetchMonster(int id)
{
    Monster *result = nullptr;
    RecordSet e = fetchRecordById("monsters", id);
    if (e.size() > 0) {
        result = new Monster();
        RecordSetIterator i(e);
        while (i.hasNext()) {
            i.next();
            result->setField(i.key(), &(i.value()));
        }
    }
    return result;
}

bool Db::startNewSession()
{
    m_lastError = "";
    QSqlQuery query("insert into sessions"
                    "(started)"
                    "values (?)");
    query.addBindValue(QDateTime::currentDateTime().toString());
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool Db::stopCurrentSession(int awardedXp)
{
    SessionInfo *info = getLatestSession();
    if (info) {
        info->awardedXp = awardedXp;
        info->stopped = QDateTime::currentDateTime();
        if (!updateSession(info)) {
            return false;
        }

        return true;
    }

    return false;
}

SessionInfo *Db::getLatestSession()
{
    m_lastError = "";
    QSqlQuery query("select id, started, stopped, awardedXp from sessions where awardedXp is null order by id");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return nullptr;
    } else {
        if (query.last()) {
            return new SessionInfo(query.value(0).toInt(),
                                   query.value(1).toString(),
                                   query.value(2).toString(),
                                   query.value(3).toInt());
        } else {
            return nullptr;
        }
    }
}

bool Db::createNewDb()
{
    if (!initializeDb()) {
        return false;
    }

    return createTables();
}

bool Db::createTables()
{
    if (!createPartiesTable()) {
        return false;
    }

    if (!createSessionsTable()) {
        return false;
    }

    if (!createEncountersTable()) {
        return false;
    }

    if (!createMonsterGroupsTable()) {
        return false;
    }

    if (!createMonstersTable()) {
        return false;
    }

    if (!createConditionsTable()) {
        return false;
    }

    return true;
}

bool Db::createPartiesTable()
{
    m_lastError = "";
    QSqlQuery query(
        "CREATE TABLE parties ("
            "id INTEGER PRIMARY KEY,"
            "size INTEGER,"
            "averageLevel INTEGER"
        ")");
/*    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }*/
    query.exec();

    return true;
}

bool Db::createEncountersTable()
{
    m_lastError = "";
    QSqlQuery query(
        "CREATE TABLE encounters ("
            "id INTEGER PRIMARY KEY,"
            "sessionsId INTEGER,"
            "totalXp INTEGER,"
            "monsterCount INTEGER,"
            "partySize INTEGER,"
            "averageLevel INTEGER,"
            "difficulty VARCHAR(10),"
            "started INTEGER DEFAULT 0,"
            "description TEXT,"
            "FOREIGN KEY (sessionsId) REFERENCES sessions(id)"
        ")");
/*    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }*/
    query.exec();

    query.exec("CREATE INDEX sessionsIdIndex ON encounters(sessionsId)");

    return true;
}

bool Db::createMonsterGroupsTable()
{
    m_lastError = "";
    QSqlQuery query(
        "CREATE TABLE monsterGroups ("
            "id INTEGER PRIMARY KEY,"
            "encounterId INTEGER NOT NULL,"
            "name VARCHAR(50),"
            "cr VARCHAR(5),"
            "number INTEGER,"
            "xp INTEGER,"
            "hp INTEGER,"
            "attackBonus INTEGER,"
            "damage INTEGER,"
            "saveDc INTEGER,"
            "armorClass INTEGER,"
            "FOREIGN KEY (encounterId) REFERENCES encounters(id)"
        ")");
/*    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }*/
    query.exec();

    query.exec("CREATE INDEX encounterIdIndex ON monsterGroups(encounterId)");

    return true;
}

bool Db::createMonstersTable()
{
    m_lastError = "";
    QSqlQuery query(
        "CREATE TABLE monsters ("
            "id INTEGER PRIMARY KEY,"
            "number INTEGER,"
            "groupId INTEGER NOT NULL,"
            "health INTEGER,"
            "FOREIGN KEY (groupId) REFERENCES monsterGroups(id)"
        ")");
    query.exec();

    query.exec("CREATE INDEX groupIdIndex ON monsters(groupId)");

    return true;
}

bool Db::createConditionsTable()
{
    m_lastError = "";
    QSqlQuery query(
        "CREATE TABLE conditions ("
            "id INTEGER PRIMARY KEY,"
            "monsterId INTEGER NOT NULL,"
            "condition VARCHAR(25),"
            "rounds INTEGER,"
            "other VARCHAR(50),"
            "FOREIGN KEY (monsterId) REFERENCES monsters(id)"
        ")");
    query.exec();

    query.exec("CREATE INDEX monsterIdIndex ON conditions(monsterId)");

    return true;
}

bool Db::createSessionsTable()
{
    m_lastError = "";
    QSqlQuery query(
                "CREATE TABLE sessions ("
                    "id	INTEGER,"
                    "started TEXT NOT NULL,"
                    "awardedXp INTEGER,"
                    "stopped TEXT,"
                    "PRIMARY KEY(id)"
        ")");
    query.exec();

    return true;
}

bool Db::deleteCondition(int id)
{
    return deleteRecordById("conditions", id);
}

bool Db::deleteMonster(int id)
{
    QList<int> list = fetchRecordsByForeignKey("conditions", "monsterId", id);
    for(int i : list) {
        if (!deleteCondition(i)) {
            return false;
        }
    }

    return deleteRecordById("monsters", id);
}

bool Db::deleteMonsterGroup(int id)
{
    QList<int> list = fetchRecordsByForeignKey("monsters", "groupId", id);
    for(int i : list) {
        if (!deleteMonster(i)) {
            return false;
        }
    }

    return deleteRecordById("monsterGroups", id);
}

bool Db::updateMonster(Monster *m)
{
    m_lastError = "";

    QString sql = "update monsters set number=?, health=?, groupId=? where id=?";
    QSqlQuery query(sql);
    query.addBindValue(m->number);
    query.addBindValue(m->health);
    query.addBindValue(m->groupId);
    query.addBindValue(m->id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool Db::updateSession(SessionInfo *info)
{
    m_lastError = "";

    QString sql = "update sessions set started=?, stopped=?, awardedXp=? where id=?";
    QSqlQuery query(sql);
    query.addBindValue(info->started.toString());
    query.addBindValue(info->stopped.toString());
    query.addBindValue(info->awardedXp);
    query.addBindValue(info->id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool Db::deleteRecordById(QString tableName, int id)
{
    m_lastError = "";

    QString sql = "delete from " + tableName + " where id=?";
    QSqlQuery query(sql);
    query.addBindValue(id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool Db::deleteAllById(QString tableName, QList<int> *list)
{
    for (int i : *list) {
        if (!deleteRecordById(tableName, i)) {
            return false;
        }
    }

    return true;
}

RecordSet Db::fetchRecordById(QString tableName, int id)
{
    RecordSet result;
    m_lastError = "";

    QString sql = "select * from " + tableName + " where id = ?";
    QSqlQuery query(sql);
    query.addBindValue(id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
    } else {
        if (query.first()) {
            for (int i = 0; i < query.record().count(); i++) {
                result.insert(query.record().fieldName(i), query.value(i));
            }
        }
    }

    return result;
}

QList<int> Db::fetchRecordsByForeignKey(QString tableName, QString foreignKeyName, int foreignKeyId)
{
    QList<int> result;
    m_lastError = "";

    QString sql = "select id from " + tableName + " where " + foreignKeyName + "=? order by id";
    QSqlQuery query(sql);
    query.addBindValue(foreignKeyId);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
    } else {
        while (query.next()) {
            result.append(query.value(0).toInt());
        }
    }

    return result;
}

bool Db::deleteEncounter(int id)
{
    QList<int> list = fetchRecordsByForeignKey("monsterGroups", "encounterId", id);
    for(int i : list) {
        if (!deleteMonsterGroup(i)) {
            return false;
        }
    }

    return deleteRecordById("encounters", id);
}

bool Db::insertMonsters(MonsterGroup *m) {
    m_lastError = "";
    for (int i = 0; i < m->number; i++) {
        QString sql = "insert into monsters "
                "(groupId, number, health)"
                "values (?, ?, ?)";
        QSqlQuery q(sql);
        q.addBindValue(m->id);
        q.addBindValue(i + 1);
        q.addBindValue(m->hp);
        if (!q.exec()) {
            m_lastError = q.lastError().text();
            return false;
        }
    }

    return true;
}

bool Db::startEncounter(int id)
{
    SessionInfo *session = getLatestSession();
    if (!session) return false;

    m_lastError = "";
    QString sql = "update encounters set started = 1, sessionsId = ? where id = ?";
    QSqlQuery query(sql);
    query.addBindValue(session->id);
    query.addBindValue(id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    Encounter *e = fetchEncounter(id);
    for (MonsterGroup *m : e->groups) {
        if (m->monsters.size() == 0 && !insertMonsters(m)) {
            return false;
        }
    }

    return true;
}

QList<Encounter *> Db::getEncountersForSession(int id)
{
    QList<Encounter *> result;
    QList<int> encIds = fetchRecordsByForeignKey("encounters", "sessionsId", id);
    for (int i : encIds) {
        result.append(fetchEncounter(i));
    }
    return result;
}

bool Db::deleteRecordsByForeingKey(QString tableName, QString foreignKeyName, int foreignKeyId)
{
    m_lastError = "";

    QString sql = "delete from " + tableName + " where " + foreignKeyName + "=?";
    QSqlQuery query(sql);
    query.addBindValue(foreignKeyId);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool Db::initializeDb()
{
    if (QSqlDatabase::isDriverAvailable("QSQLITE")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbFile.absoluteFilePath());
        QDir dir(dbFile.absolutePath());
        if (!dir.exists()) {
            dir.mkdir(dbFile.absolutePath());
        }
        return db.open();
    }

    return false;
}

