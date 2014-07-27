#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class AddbookDialog;
}

class AddbookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddbookDialog(QWidget *parent = 0);
    ~AddbookDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddbookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
