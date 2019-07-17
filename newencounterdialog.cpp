#include "encountermodel.h"
#include "newencounterdialog.h"
#include "ui_newencounterdialog.h"

#include <QDebug>

static int xpPerCr[] = {
    25, 50, 100, 200, 450, 700, 1100, 1800,
    2300, 2900, 3900, 5000, 5900,
    7200, 8400, 10000, 11500, 13000,
    15000, 18000, 20000, 22000, 25000,
    33000, 41000, 50000, 62000, 75000,
    90000, 105000, 120000, 135000, 155000
};

static QString hpPerCr[] = {
    "7-35", "36-49", "50-70",
    "71-85", "86-100", "101-115", "116-130", "131-145",
    "146-160", "161-175", "176-190", "191-205", "206-220",
    "221-235", "236-250", "251-265", "266-280", "281-295",
    "296-310", "311-325","326-340","341-355","356-400",
    "401-445","446-490","491-535","536-580","581-625",
    "626-670","671-715","716-760","761-805","805-850"
};

static int acPerCr[] = {
    13, 13, 13,
    13, 13, 13, 14, 15,
    15, 15, 16, 16, 17,
    17, 17, 18, 18, 18,
    18, 19, 19, 19, 19,
    19, 19, 19, 19, 19,
    19, 19, 19, 19, 19
};

static int abPerCr[] = {
    3, 3, 3,
    3, 3, 4, 5, 6,
    6, 6, 7, 7, 7,
    8, 8, 8, 8, 8,
    9, 10, 10, 10, 10,
    11, 11, 11, 12, 12,
    12, 13, 13, 13, 14
};

static QString dmgPerCr[] = {
    "2-3","4-5","6-8",
    "9-14","15-20","21-26","27-32","33-38",
    "39-44","45-50","51-56","57-62","63-58",
    "69-74","75-80","81-86","87-92","93-98",
    "99-104","105-110","111-116","117-122","123-140",
    "141-158","159-176","177-194","195-212","213-230",
    "231-248","249-266","267-284","285-302","303-320"
};

static int savePerCr[] = {
    13,13,13,
    13,13,13,14,15,
    15,15,16,16,16,
    17,17,18,18,18,
    18,19,19,19,19,
    20,20,20,21,21,
    21,22,22,22,23
};

static int xpThreasholds[20][4] = {
    {25, 50, 75, 100},
    {50, 100, 150, 200},
    {75, 150, 225, 400},
    {125, 250, 375, 500},
    {250, 500, 750, 1100},
    {300, 600, 900, 1400},
    {350, 750, 1100, 1700},
    {450, 900, 1400, 2100},
    {550, 1100, 1600, 2400},
    {600, 1200, 1900, 2800},
    {800, 1600, 2400, 3600},
    {1000, 2000, 3000, 4500},
    {1100, 2200, 3400, 5100},
    {1250, 2500, 3800, 5700},
    {1400, 2800, 4300, 6400},
    {1600, 3200, 4800, 7200},
    {2000, 3900, 5900, 8800},
    {2100, 4200, 6300, 9500},
    {2400, 4900, 7300, 10900},
    {2800, 5700, 8500, 12700}
};

NewEncounterDialog::NewEncounterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEncounterDialog)
{
    ui->setupUi(this);
    ui->descriptionEdit->setText("");

    m_model = new EncounterModel();
    ui->monsterGroupsTable->setModel(m_model);

    reset();
}

NewEncounterDialog::~NewEncounterDialog()
{
    delete ui;
}

void NewEncounterDialog::done(int res)
{
  if (res == QDialog::Accepted)
  {
      m_partySize = ui->partySizeSpin->value();
      m_averageLevel = ui->averageLevelSpin->value();

      model()->setPartyData(m_partySize, m_averageLevel);
      model()->difficulty = ui->difficultyEdit->text();
      model()->description = ui->descriptionEdit->text();
  } else {
      delete m_model;
      m_model = nullptr;
  }

  QDialog::done(res);
}

void NewEncounterDialog::setPartyData(int size, int averageLevel)
{
    m_partySize = size;
    m_averageLevel = averageLevel;

    ui->partySizeSpin->setValue(m_partySize);
    ui->averageLevelSpin->setValue(m_averageLevel);
}

void NewEncounterDialog::on_crComboBox_currentIndexChanged(int index)
{
    updateMonsterStats(index);
}

QString NewEncounterDialog::calculateAverageFromRange(QString range)
{
    QStringList list = range.split("-");
    if (list.size() != 2) {
        return "";
    } else {
        int a = list[0].toInt();
        int b = list[1].toInt();
        return QString::number(int(((b - a) / 2) + a));
    }
}

void NewEncounterDialog::updateMonsterStats(int index)
{
    ui->hpLabel->setText(hpPerCr[index]);
    ui->hitPointsEdit->setText(calculateAverageFromRange(hpPerCr[index]));
    ui->armorClassLabel->setText(QString::number(acPerCr[index]));
    ui->armorClassEdit->setText(QString::number(acPerCr[index]));
    ui->attkBonusLabel->setText("+" + QString::number(abPerCr[index]));
    ui->attkBonusEdit->setText("+" + QString::number(abPerCr[index]));
    ui->baseDmgLabel->setText(dmgPerCr[index]);
    ui->baseDmgEdit->setText(calculateAverageFromRange(dmgPerCr[index]));
    ui->saveDcLabel->setText("+" + QString::number(savePerCr[index]));
    ui->saveDcEdit->setText("+" + QString::number(savePerCr[index]));

    updateXpTotals();
}

void NewEncounterDialog::updateXpTotals()
{
    int totalBaseXp = 0;
    int numMonsters = ui->numberSpin->value();

    // Update for monster group being edited
    if (numMonsters > 0) {
        int crIndex = ui->crComboBox->currentIndex();
        totalBaseXp = xpPerCr[crIndex] * numMonsters;
        ui->xpEdit->setText(QLocale(QLocale::English).toString(totalBaseXp));
    } else {
        ui->xpEdit->setText("0");
    }

    numMonsters += m_model->monsterCount();
    totalBaseXp += m_model->xpTotal();

    int numPlayers = ui->partySizeSpin->value();
    int averageLevel = ui->averageLevelSpin->value();
    int adjustedXp = totalBaseXp;

    if (numMonsters == 1) {
        // do nothing
    } else if (numMonsters == 2) {
        adjustedXp *= 1.5;
    } else if (numMonsters < 7) {
        adjustedXp *= 2;
    } else if (numMonsters < 11) {
        adjustedXp *= 2.5;
    } else if (numMonsters < 15) {
        adjustedXp *= 3;
    } else {
        adjustedXp *= 4;
    }

    ui->adjustedXpEdit->setText(QLocale(QLocale::English).toString(adjustedXp));
    ui->xpPerPlayerEdit->setText(QLocale(QLocale::English).toString(adjustedXp/numPlayers));
    ui->difficultyEdit->setText(calculateDifficulty(numPlayers, averageLevel, adjustedXp));
    if (ui->difficultyEdit->text() == "Deadly") {
        ui->difficultyEdit->setStyleSheet("color: red");
    } else {
        ui->difficultyEdit->setStyleSheet("color: white");
    }
}

QString NewEncounterDialog::calculateDifficulty(int partySize, int averageLevel, int totalXp)
{
    int *levelDifficulty = xpThreasholds[averageLevel - 1];
    if (totalXp < levelDifficulty[1] * partySize) {
        return "Easy";
    } else if (totalXp < levelDifficulty[2] * partySize) {
        return "Medium";
    } else if (totalXp < levelDifficulty[3] * partySize) {
        return "Hard";
    } else {
        return "Deadly";
    }
}

void NewEncounterDialog::reset()
{
    ui->nameEdit->setText("");
    ui->numberSpin->setValue(0);
    ui->crComboBox->setCurrentIndex(0);
    updateMonsterStats(0);
}

void NewEncounterDialog::on_partySizeSpin_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateXpTotals();
}

void NewEncounterDialog::on_numberSpin_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateXpTotals();
}

void NewEncounterDialog::on_pushButton_clicked()
{
    if (!ui->nameEdit->text().isEmpty() && ui->numberSpin->value() > 0) {
        MonsterGroup *monsterGroup = new MonsterGroup();
        monsterGroup->name = ui->nameEdit->text();
        monsterGroup->cr = ui->crComboBox->currentText();
        monsterGroup->hp = ui->hitPointsEdit->text().toInt();
        monsterGroup->xp = ui->xpEdit->text().remove(',').toInt();
        monsterGroup->damage = ui->baseDmgEdit->text().toInt();
        monsterGroup->number = ui->numberSpin->value();
        monsterGroup->saveDc = ui->saveDcEdit->text().toInt();
        monsterGroup->armorClass = ui->armorClassEdit->text().toInt();
        monsterGroup->attackBonus = ui->attkBonusEdit->text().toInt();
        m_model->add(monsterGroup);
        m_model->description = ui->descriptionEdit->text();
        reset();
    }
}

void NewEncounterDialog::on_averageLevelSpin_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateXpTotals();
}

void NewEncounterDialog::on_pushButton_2_clicked()
{
    QItemSelectionModel *select = ui->monsterGroupsTable->selectionModel();
    qDebug() << select->currentIndex().row();
    if (select->currentIndex().row() > -1) {
        model()->removeRow(select->currentIndex().row(), QModelIndex());
    }
    updateXpTotals();
}
