#include "startencounterdialog.h"
#include "ui_startencounterdialog.h"

#include "db/db.h"

#include <QDebug>

StartEncounterDialog::StartEncounterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartEncounterDialog)
{
    ui->setupUi(this);

//    ui->encountersList->setStyleSheet("color: #000000");

    loadEncounters();
}

StartEncounterDialog::~StartEncounterDialog()
{
    delete ui;
}

void StartEncounterDialog::done(int res)
{
  if (res == QDialog::Accepted)
  {
      QList<QListWidgetItem *> items = ui->encountersList->selectedItems();
      if (items.size() > 0) {
          QListWidgetItem *item = items.first();
          m_selectedEncounterId = item->data(Qt::UserRole).toInt();
      }
  } else
  {
      m_selectedEncounterId = -1;
  }

  QDialog::done(res);
}

void StartEncounterDialog::loadEncounters(bool filterStarted)
{
    ui->encountersList->clear();
    QList<EncounterInfo *> list;
    if (filterStarted)
    {
        list = db->fetchNonStartedEncounters();
    } else {
        list = db->fetchAllEncounters();
    }
    for(EncounterInfo *e : list) {
        QListWidgetItem *item = new QListWidgetItem(ui->encountersList);
        if (e->started) {
            item->setText(e->description + " (in progress)");
        } else {
            item->setText(e->description);
        }
        item->setData(Qt::UserRole, e->id);
        ui->encountersList->addItem(item);
    }
}

void StartEncounterDialog::on_pushButton_clicked()
{
    QList<QListWidgetItem *> items = ui->encountersList->selectedItems();
    if (items.size() > 0) {
        QListWidgetItem *item = items.first();
        db->deleteEncounter(item->data(Qt::UserRole).toInt());
        loadEncounters(!ui->showAllCheckBox->checkState());
    }
}


void StartEncounterDialog::on_showAllCheckBox_stateChanged(int arg1)
{
    if (arg1) {
        loadEncounters(false);
    } else {
        loadEncounters(true);
    }
}
