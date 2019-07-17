#ifndef STARTENCOUNTERDIALOG_H
#define STARTENCOUNTERDIALOG_H

#include <QDialog>

namespace Ui {
class StartEncounterDialog;
}

class StartEncounterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartEncounterDialog(QWidget *parent = nullptr);
    ~StartEncounterDialog();

    void done(int res);

    int selectedEncounterId() const { return m_selectedEncounterId; }

private slots:
    void on_pushButton_clicked();

    void on_showAllCheckBox_stateChanged(int arg1);

private:
    Ui::StartEncounterDialog *ui;

    int m_selectedEncounterId;

    void loadEncounters(bool filterStarted = true);
};

#endif // STARTENCOUNTERDIALOG_H
