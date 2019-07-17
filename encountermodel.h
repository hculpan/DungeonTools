#ifndef ENCOUNTERMODEL_H
#define ENCOUNTERMODEL_H

#include "db/monstergroup.h"

#include <QAbstractTableModel>

class EncounterModel : public QAbstractTableModel
{
private:
    QList<MonsterGroup *> m_monsterGroups;

    int m_partySize;
    int m_averageLevel;

public:
    EncounterModel();
    ~EncounterModel();

    void add(MonsterGroup *monsterGroup);

    int rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return m_monsterGroups.size(); }
    int columnCount(const QModelIndex &parent) const { Q_UNUSED(parent); return MonsterGroup::columnCount(); }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    virtual bool removeRows(int row, int count, const QModelIndex &parent);

    int xpTotal();
    int monsterCount();

    QList<MonsterGroup *> monsterGroups() const { return m_monsterGroups; }

    void setPartyData(int size, int averageLevel) { m_partySize = size; m_averageLevel = averageLevel; }

    int partySize() const { return m_partySize; }
    int averageLevel() const { return m_averageLevel; }

    QString description;

    QString difficulty;
};

#endif // ENCOUNTERMODEL_H
