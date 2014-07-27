#include "returnbookdialog.h"
#include "ui_returnbookdialog.h"
#include <QtSql>

ReturnbookDialog::ReturnbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnbookDialog)
{
    ui->setupUi(this);
}

ReturnbookDialog::~ReturnbookDialog()
{
    delete ui;
}

void ReturnbookDialog::on_buttonBox_accepted()
{
    bid = ui->lineEdit_bid->text();
    rid = ui->lineEdit_rid->text();
}
