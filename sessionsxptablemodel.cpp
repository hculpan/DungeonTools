#include "sessionsxptablemodel.h"

#include <db/db.h>

#include <QString>
#include <QLocale>

SessionsXpTableModel::SessionsXpTableModel(QObject *parent) : QAbstractTableModel (parent)
{

}

QVariant SessionsXpTableModel::data(const QModelIndex &index, int role) const
{
    if (index.column() != 0 && role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    Encounter *e = m_encounters[index.row()];
    if (e) {
        switch (index.column()) {
        case 0:
            return QVariant(e->description);
        case 1:
            return QVariant(QLocale(QLocale::English).toString(e->totalXp));
        case 2:
            return QVariant("");
        }
    }


    return QVariant();
}

QVariant SessionsXpTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return sessionsXpFields[section];
    }

    return QVariant();
}

void SessionsXpTableModel::setSessionId(int id)
{
    beginResetModel();
    m_encounters = db->getEncountersForSession(id);
    endResetModel();
}

