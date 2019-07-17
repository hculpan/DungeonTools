#ifndef DB_H
#define DB_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

#include "encountermodel.h"
#include "db/monstergroup.h"
#include "db/encounter.h"

typedef QMap<QString, QVariant>         RecordSet;
typedef QMapIterator<QString, QVariant> RecordSetIterator;

struct PartyData {
    int size;
    int averageLevel;

    PartyData(int size, int averageLevel) {
        this->size = size;
        this->averageLevel = averageLevel;
    }
};

struct SessionInfo {
    int id;
    QDateTime started;
    int awardedXp;
    QDateTime stopped;

    SessionInfo(int id, QDateTime started) {
        this->id = id;
        this->started = started;
    }

    SessionInfo(int id, QString started) {
        this->id = id;
        this->started = QDateTime::fromString(started);
    }

    SessionInfo(int id, QString started, QString stopped, int awardedXp) {
        this->id = id;
        this->started = QDateTime::fromString(started);
        this->stopped = QDateTime::fromString(stopped);
        this->awardedXp = awardedXp;
    }
};

struct EncounterInfo {
    QString description;
    int id;
    bool started;

    EncounterInfo(QString description, int id, bool started = false) {
        this->description = description;
        this->id = id;
        this->started = started;
    }

    EncounterInfo(QString description, int id, int started) {
        this->description = description;
        this->id = id;
        this->started = (started > 0);
    }
};

class Db : public QObject
{
    Q_OBJECT

private:
    QFileInfo dbFile;

    QSqlDatabase db;

    QString m_lastError;

    bool createNewDb();

    bool createTables();

    bool createPartiesTable();
    bool createEncountersTable();
    bool createMonsterGroupsTable();
    bool createMonstersTable();
    bool createConditionsTable();
    bool createSessionsTable();

    bool deleteCondition(int id);
    bool deleteMonster(int id);
    bool deleteMonsterGroup(int id);

    bool deleteRecordsByForeingKey(QString tableName, QString foreignKeyName, int foreignKeyId);
    bool deleteRecordById(QString tableName, int id);
    bool deleteAllById(QString tableName, QList<int> *list);

    RecordSet fetchRecordById(QString tableName, int id);

    bool insertMonsters(MonsterGroup *m);

    QList<int> fetchRecordsByForeignKey(QString tableName, QString foreignKeyName, int foreignKeyId);

    bool initializeDb();

public:
    explicit Db(QFileInfo fileInfo, QObject *parent = nullptr);
    ~Db();

    QString lastError() const { return m_lastError; }

    bool updatePartyData(PartyData *partyData);
    PartyData *fetchPartyData();

    bool addEncounter(EncounterModel *encounter);
    bool addMonsterGroup(int encounterId, MonsterGroup *m);

    QList<EncounterInfo *> fetchNonStartedEncounters();
    QList<EncounterInfo *> fetchAllEncounters();

    Encounter *fetchEncounter(int id);
    MonsterGroup *fetchMonsterGroup(int id);
    Monster *fetchMonster(int id);

    bool updateMonster(Monster *m);
    bool updateSession(SessionInfo *info);

    bool deleteEncounter(int id);
    bool startEncounter(int id);

    QList<Encounter *> getEncountersForSession(int id);

    bool startNewSession();
    bool stopCurrentSession(int awardedXp = 0);
    SessionInfo *getLatestSession();

signals:

public slots:
};

extern Db *db;

#endif // DB_H
