#ifndef ADDREADERDIALOG_H
#define ADDREADERDIALOG_H

#include <QDialog>

namespace Ui {
class AddreaderDialog;
}

class AddreaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddreaderDialog(QWidget *parent = 0);
    ~AddreaderDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddreaderDialog *ui;
};

#endif // ADDREADERDIALOG_H
