#ifndef ACTIVEENCOUNTERDELEGATE_H
#define ACTIVEENCOUNTERDELEGATE_H

#include <QItemDelegate>

class ActiveEncounterDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ActiveEncounterDelegate(QObject *parent = 0);
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // ACTIVEENCOUNTERDELEGATE_H
