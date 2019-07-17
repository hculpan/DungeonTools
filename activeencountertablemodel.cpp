#include "activeencountertablemodel.h"

#include <QBrush>

#include "db/db.h"

#include <QDebug>

ActiveEncounterTableModel::ActiveEncounterTableModel(Encounter *encounter, QObject *parent) : QAbstractTableModel (parent)
{
    this->m_encounter = encounter;
}

QVariant ActiveEncounterTableModel::data(const QModelIndex &index, int role) const
{
    if (index.column() != 1 && role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    if (role == Qt::BackgroundRole) {
        Monster *m = m_encounter->getMonsterAt(index.row());
        if (m->health <= 0) {
            return QBrush(Qt::red);
        }
    }

    if (!index.isValid() || role != Qt::DisplayRole || !m_encounter || index.row() >= m_encounter->monsterCount) {
        return QVariant();
    }

    Monster *m = m_encounter->getMonsterAt(index.row());
    MonsterGroup *group = m_encounter->getMonsterGroupFor(m);

    switch (index.column()) {
    case 0:
        if (group->number == 1) {
            return QVariant("-");
        } else {
            return QVariant(m->number);
        }
    case 1:
        return QVariant(group->name);
    case 2:
        return QVariant(group->hp);
    case 3:
        return QVariant(m->health);
    case 4:
        return QVariant(group->armorClass);
    case 5:
        return QVariant("+" + QString::number(group->attackBonus));
    case 6:
        return QVariant(group->damage);
    case 7:
        return QVariant(group->saveDc);
    case 8:
        return QVariant(group->cr);
    case 9:
        return QVariant(QLocale(QLocale::English).toString(int(group->xp/group->number)));
    case 10:
        return QVariant("-");
    case 11:
        return QVariant("+");
    }

    return QVariant();
}

QVariant ActiveEncounterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return activeEncounterFields[section];
    }

    return QVariant();
}

void ActiveEncounterTableModel::setEncounter(Encounter *encounter)
{
    beginResetModel();
    if (m_encounter) {
        delete m_encounter;
    }
    m_encounter = encounter;
    endResetModel();
}

void ActiveEncounterTableModel::updateMonsterHealth(Monster *m, int change)
{
    beginResetModel();
    m->health += change;
    if (m->health < 0) {
        m->health = 0;
    } else if (m->health > m_encounter->getMonsterGroupFor(m)->hp) {
        m->health = m_encounter->getMonsterGroupFor(m)->hp;
    }
    db->updateMonster(m);
    endResetModel();
}


