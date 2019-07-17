#include "encountermodel.h"


#include <QDebug>
EncounterModel::EncounterModel()
{
}

EncounterModel::~EncounterModel()
{
    for(MonsterGroup *m : m_monsterGroups) {
        delete m;
    }
}

QVariant EncounterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || index.row() >= m_monsterGroups.size()) {
        return QVariant();
    }

    MonsterGroup *monsterGroup = m_monsterGroups[index.row()];

    switch (index.column()) {
    case 0:
        return QVariant(monsterGroup->name);
    case 1:
        return QVariant(monsterGroup->number);
    case 2:
        return QVariant(monsterGroup->cr);
    case 3:
        return QVariant(monsterGroup->xp);
    case 4:
        return QVariant(monsterGroup->hp);
    case 5:
        return QVariant(monsterGroup->armorClass);
    case 6:
        return QVariant(monsterGroup->attackBonus);
    case 7:
        return QVariant(monsterGroup->damage);
    case 8:
        return QVariant(monsterGroup->saveDc);
    }

    return QVariant();
}

QVariant EncounterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QVariant("Name");
        case 1:
            return QVariant("Number");
        case 2:
            return QVariant("CR");
        case 3:
            return QVariant("XP");
        case 4:
            return QVariant("HP");
        case 5:
            return QVariant("AC");
        case 6:
            return QVariant("Attk");
        case 7:
            return QVariant("Dmg");
        case 8:
            return QVariant("DC");
        }
    }

    return QVariant();
}

bool EncounterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    beginRemoveRows(parent, row, row);
    m_monsterGroups.removeAt(row);
    endRemoveRows();
    return true;
}

int EncounterModel::xpTotal()
{
    int result = 0;
    for(MonsterGroup *m : m_monsterGroups) {
        result += m->xp;
    }
    return result;
}

int EncounterModel::monsterCount()
{
    int result = 0;
    for(MonsterGroup *m : m_monsterGroups) {
        result += m->number;
    }
    return result;
}

void EncounterModel::add(MonsterGroup *monsterGroup)
{
    //QModelIndex parent(QAbstractItemModel::createIndex(m_monsterGroups.size() - 1, 0));
    beginInsertRows(QModelIndex(), m_monsterGroups.size(), m_monsterGroups.size());
    m_monsterGroups.append(monsterGroup);
    endInsertRows();
}
