#ifndef RETURNBOOKDIALOG_H
#define RETURNBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class ReturnbookDialog;
}

class ReturnbookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnbookDialog(QWidget *parent = 0);
    ~ReturnbookDialog();
    QString get_bid()
    {
        return bid;
    }
    QString get_rid()
    {
        return rid;
    }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ReturnbookDialog *ui;
    QString bid;
    QString rid;
};

#endif // RETURNBOOKDIALOG_H
