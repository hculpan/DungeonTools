#ifndef ACTIVEENCOUNTERTABLEMODEL_H
#define ACTIVEENCOUNTERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include "db/encounter.h"

#define ACTIVE_ENCOUNTER_FIELD_COUNT    12

static QString activeEncounterFields[ACTIVE_ENCOUNTER_FIELD_COUNT] = {
    "#",
    "Name",
    "Base HP",
    "Health",
    "AC",
    "Attk",
    "Dmg",
    "DC",
    "CR",
    "XP",
    "Hurt",
    "Heal"
};

class ActiveEncounterTableModel : public QAbstractTableModel
{
public:
    ActiveEncounterTableModel(Encounter *encounter, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return (m_encounter ? m_encounter->monsterCount : 0); }
    int columnCount(const QModelIndex &parent) const { Q_UNUSED(parent); return ACTIVE_ENCOUNTER_FIELD_COUNT; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Encounter *encounter() const { return m_encounter; }
    void setEncounter(Encounter *encounter);

    void updateMonsterHealth(Monster *m, int change);

private:
    Encounter *m_encounter;
};

#endif // ACTIVEENCOUNTERTABLEMODEL_H
