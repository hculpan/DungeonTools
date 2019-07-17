#include "activeencounterdelegate.h"

#include <QtGui>
#include <QDialog>
#include <QApplication>
#include <QTableView>
#include <QInputDialog>

#include "activeencounterdelegate.h"
#include "activeencountertablemodel.h"

#include <QDebug>

ActiveEncounterDelegate::ActiveEncounterDelegate(QObject *parent) : QItemDelegate (parent)
{

}

/*void ActiveEncounterDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() > 9) {
        QStyleOptionButton button;
        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 30;//the X coordinate
        y = r.top();//the Y coordinate
        w = 30;//button width
        h = 30;//button height
        button.rect = QRect(x,y,w,h);
        button.text = (index.column() == 10 ? "-" : "+");
        button.state = QStyle::State_Enabled;

        QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}*/

bool ActiveEncounterDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(model);
    Q_UNUSED(option);

    if( event->type() == QEvent::MouseButtonRelease )
    {
        QTableView *t = dynamic_cast<QTableView *>(parent());
        ActiveEncounterTableModel *model = dynamic_cast<ActiveEncounterTableModel *>(t->model());
        Monster *m = model->encounter()->getMonsterAt(index.row());
        if (index.column() == 10) {  // '+' clicked
            bool ok;
            QString title = model->encounter()->getMonsterGroupFor(m)->name + " " + QString::number(m->number);
            QString text = QInputDialog::getText(t, title, tr("Amount of damage:"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                model->updateMonsterHealth(m, text.toInt() * -1);
            }
        } else if (index.column() == 11) {
            bool ok;
            QString title = model->encounter()->getMonsterGroupFor(m)->name + " " + QString::number(m->number);
            QString text = QInputDialog::getText(t, title, tr("Amount to heal:"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                model->updateMonsterHealth(m, text.toInt());
            }
        }
    }

    return true;
}

