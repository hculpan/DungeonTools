#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newencounterdialog.h"
#include "startencounterdialog.h"
#include "activeencountertablemodel.h"
#include "activeencounterdelegate.h"
#include "sessionxpdialog.h"

#include "settings.h"
#include "db/db.h"

#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sessionInfo = db->getLatestSession();

    Encounter *encounter = nullptr;
    if (settings->currentEncounterId > -1 && sessionInfo) {
        encounter = db->fetchEncounter(settings->currentEncounterId);
        if (!encounter->started) {
            delete encounter;
            encounter = nullptr;
        }
    }

    ui->setupUi(this);
    this->setFixedSize(QSize(800, 600));
    this->move(settings->mainWindowX, settings->mainWindowY);

    ui->encounterTableView->setModel(new ActiveEncounterTableModel(encounter, this));
    for (int i = 0; i < ACTIVE_ENCOUNTER_FIELD_COUNT - 1; i++) {
        int width = 55;
        if (i == 0) {
            width = 30;
        } else if (i == 1) {
            width = 200;
        } else if (i == 9) {
            width = 100;
        }
        ui->encounterTableView->setColumnWidth(i, width);
    }
    int remainingSpace = this->geometry().width() - (55 * 7 + 335);
    ui->encounterTableView->setColumnWidth(10, remainingSpace / 2);
    ui->encounterTableView->setColumnWidth(11, remainingSpace / 2);
    ui->encounterTableView->setItemDelegate(new ActiveEncounterDelegate(ui->encounterTableView));


    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete sessionInfo;
    delete ui;
}

void MainWindow::writeSettings() {
    settings->mainWindowX = this->geometry().x();
    settings->mainWindowY = this->geometry().y();
    settings->writeSettings();
}

void MainWindow::updateStatusBar()
{
    QString status;
    if (sessionInfo) {
        status = QString("Session " + QString::number(sessionInfo->id) + " started " + sessionInfo->started.toString("MMM d, yyyy h:mm A")).leftJustified(75);

        if (encounter()) {
            status += "Encounter: " + encounter()->description;
        }
    } else {
        status = "No active session";
    }

    ui->statusBar->showMessage(status);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (true) {
        writeSettings();
        event->accept();
//    } else {
//        event->ignore();
    }
}

Encounter *MainWindow::encounter()
{
    ActiveEncounterTableModel *a = dynamic_cast<ActiveEncounterTableModel *>(ui->encounterTableView->model());
    return a->encounter();
}

void MainWindow::on_pushButton_clicked()
{
    NewEncounterDialog *dialog = new NewEncounterDialog(this);

    PartyData *partyData = db->fetchPartyData();
    if (partyData) {
        dialog->setPartyData(partyData->size, partyData->averageLevel);
        delete partyData;
    }

    dialog->setModal(true);
    dialog->setFixedSize(459, 575);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
    if (dialog->model()) {
        PartyData partyData(dialog->partySize(), dialog->averageLevel());
        if (!db->updatePartyData(&partyData)) {
            qInfo() << "Unable to update parties info: " << db->lastError();
        }
        if (!db->addEncounter(dialog->model())) {
            qInfo() << "Unable to update encounter info: " << db->lastError();
        }
        delete dialog->model();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!db->getLatestSession()) {
        QMessageBox msgBox;
        msgBox.setText("There is not active session.");
        msgBox.setInformativeText("Do you want to open a new session?");
        msgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Open);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Open:
            on_pushButton_3_clicked();
            break;
        case QMessageBox::Cancel:
            return;
        default:
            // should never be reached
            break;
        }
    }

    StartEncounterDialog *dialog = new StartEncounterDialog(this);

    dialog->setModal(true);
    dialog->setFixedSize(371, 500);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
    if (dialog->selectedEncounterId() >= 0) {
        db->startEncounter(dialog->selectedEncounterId());
        settings->currentEncounterId = dialog->selectedEncounterId();
        Encounter *encounter = db->fetchEncounter(dialog->selectedEncounterId());
        ActiveEncounterTableModel *a = dynamic_cast<ActiveEncounterTableModel *>(ui->encounterTableView->model());
        a->setEncounter(encounter);
        updateStatusBar();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    db->stopCurrentSession();
    db->startNewSession();
    ActiveEncounterTableModel *a = dynamic_cast<ActiveEncounterTableModel *>(ui->encounterTableView->model());
    a->setEncounter(nullptr);

    sessionInfo = db->getLatestSession();
    updateStatusBar();
}

void MainWindow::on_pushButton_4_clicked()
{
    SessionInfo *info = db->getLatestSession();
    if (!info) {
        QMessageBox msgBox;
        msgBox.setText("No active session");
        msgBox.exec();
        return;
    }

    SessionXpDialog *dialog = new SessionXpDialog(info->id, this);

    dialog->setModal(true);
    dialog->exec();
    if (dialog->awardedXp() > -1) {
        ActiveEncounterTableModel *a = dynamic_cast<ActiveEncounterTableModel *>(ui->encounterTableView->model());
        a->setEncounter(nullptr);
        db->stopCurrentSession(dialog->awardedXp());
        delete sessionInfo;
        sessionInfo = nullptr;
        updateStatusBar();
    }
}
