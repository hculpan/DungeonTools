#ifndef SESSIONXPDIALOG_H
#define SESSIONXPDIALOG_H

#include <QDialog>

namespace Ui {
class SessionXpDialog;
}

class SessionXpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionXpDialog(int sessionId, QWidget *parent = nullptr);
    ~SessionXpDialog();

    int awardedXp() const { return m_awardedXp; }

    void done(int res);

private slots:
    void on_xpBonusEdit_textChanged(const QString &arg1);

private:
    Ui::SessionXpDialog *ui;

    int m_awardedXp;

    void updateInterface();
};

#endif // SESSIONXPDIALOG_H
