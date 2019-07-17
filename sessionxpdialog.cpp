#include "sessionxpdialog.h"
#include "ui_sessionxpdialog.h"

#include "sessionsxptablemodel.h"
#include "db/db.h"

SessionXpDialog::SessionXpDialog(int sessionId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionXpDialog)
{
    ui->setupUi(this);
    SessionsXpTableModel *model = new SessionsXpTableModel(this);
    model->setSessionId(sessionId);
    ui->encountersTableView->setModel(model);

    PartyData *partyData = db->fetchPartyData();
    if (partyData) {
        ui->playersSpin->setValue(partyData->size);
        delete partyData;
    }

    m_awardedXp = -1;

    updateInterface();
}

SessionXpDialog::~SessionXpDialog()
{
    delete ui;
}

void SessionXpDialog::done(int res)
{
  if (res == QDialog::Accepted)
  {
      SessionsXpTableModel *model = dynamic_cast<SessionsXpTableModel *>(ui->encountersTableView->model());
      int totalXp = 0;
      for(Encounter *e : *model->encounters()) {
          totalXp += e->totalXp;
      }

      ui->encountersXpEdit->setText(QLocale(QLocale::English).toString(totalXp));
      int bonusXp = ui->xpBonusEdit->text().toInt();
      m_awardedXp = totalXp + bonusXp;
  } else {
      m_awardedXp = -1;
  }

  QDialog::done(res);
}

void SessionXpDialog::updateInterface()
{
    SessionsXpTableModel *model = dynamic_cast<SessionsXpTableModel *>(ui->encountersTableView->model());
    int totalXp = 0;
    for(Encounter *e : *model->encounters()) {
        totalXp += e->totalXp;
    }

    ui->encountersXpEdit->setText(QLocale(QLocale::English).toString(totalXp));
    int bonusXp = ui->xpBonusEdit->text().toInt();
    int numPlayers = ui->playersSpin->value();
    ui->playerXpEdit->setText(QLocale(QLocale::English).toString(int((totalXp + bonusXp)/numPlayers)));
}

void SessionXpDialog::on_xpBonusEdit_textChanged(const QString &arg1)
{
    updateInterface();
}
