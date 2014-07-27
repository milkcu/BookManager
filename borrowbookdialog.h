#ifndef BORROWBOOKDIALOG_H
#define BORROWBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class BorrowbookDialog;
}

class BorrowbookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowbookDialog(QWidget *parent = 0);
    ~BorrowbookDialog();
    void set_bid(QString bid_)
    {
        bid = bid_;
    }
    QString get_rid()
    {
        return rid;
    }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BorrowbookDialog *ui;
    QString bid;
    QString rid;
};

#endif // BORROWBOOKDIALOG_H
