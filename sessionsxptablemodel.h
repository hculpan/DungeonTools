#ifndef SESSIONSXPTABLEMODEL_H
#define SESSIONSXPTABLEMODEL_H

#include "db/encounter.h"

#include <QAbstractTableModel>

#define SESSIONS_XP_FIELD_COUNT    3

static QString sessionsXpFields[SESSIONS_XP_FIELD_COUNT] = {
    "Encounter",
    "XP",
    ""
};


class SessionsXpTableModel : public QAbstractTableModel
{
public:
    SessionsXpTableModel(QObject *parent);

    int rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return m_encounters.size(); }
    int columnCount(const QModelIndex &parent) const { Q_UNUSED(parent); return SESSIONS_XP_FIELD_COUNT; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setSessionId(int id);

    QList<Encounter *> *encounters() { return &m_encounters; }

private:
    QList<Encounter *> m_encounters;
};

#endif // SESSIONSXPTABLEMODEL_H
