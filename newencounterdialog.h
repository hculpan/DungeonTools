#ifndef NEWENCOUNTERDIALOG_H
#define NEWENCOUNTERDIALOG_H

#include "encountermodel.h"

#include <QDialog>

namespace Ui {
class NewEncounterDialog;
}

class NewEncounterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewEncounterDialog(QWidget *parent = nullptr);
    ~NewEncounterDialog();

    void done(int res);
    EncounterModel *model() const { return m_model; }

    int partySize() const { return m_partySize; }
    int averageLevel() const { return m_averageLevel; }

    void setPartyData(int size, int averageLevel);

private slots:
    void on_crComboBox_currentIndexChanged(int index);

    void on_partySizeSpin_valueChanged(int arg1);

    void on_numberSpin_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_averageLevelSpin_valueChanged(int arg1);

    void on_pushButton_2_clicked();

private:
    Ui::NewEncounterDialog *ui;
    EncounterModel *m_model;

    int m_partySize = 1;
    int m_averageLevel = 1;

    QString calculateAverageFromRange(QString range);
    void updateMonsterStats(int index);
    void updateXpTotals();
    QString calculateDifficulty(int partySize, int averageLevel, int totalXp);
    void reset();
};

#endif // NEWENCOUNTERDIALOG_H
