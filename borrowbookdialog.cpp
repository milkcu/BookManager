#include "borrowbookdialog.h"
#include "ui_borrowbookdialog.h"
#include <QtSql>
#include <QDate>

BorrowbookDialog::BorrowbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowbookDialog)
{
    ui->setupUi(this);
}

BorrowbookDialog::~BorrowbookDialog()
{
    delete ui;
}

void BorrowbookDialog::on_buttonBox_accepted()
{
    rid = ui->lineEdit_rid->text();
}
