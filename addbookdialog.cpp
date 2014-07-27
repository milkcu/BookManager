#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include <QMessageBox>
#include <QtSql>

AddbookDialog::AddbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddbookDialog)
{
    ui->setupUi(this);
}

AddbookDialog::~AddbookDialog()
{
    delete ui;
}

void AddbookDialog::on_buttonBox_accepted()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mbook.db");
    db.open();
    QString isbn = ui->lineEdit_isbn->text();
    QString title = ui->lineEdit_title->text();
    QString author = ui->lineEdit_author->text();
    QString press = ui->lineEdit_press->text();
    QString pub = ui->lineEdit_pub->text();
    QString price = ui->lineEdit_price->text();
    QString copynum = ui->lineEdit_copynum->text();
    QString classnum = ui->lineEdit_classnum->text();
    QSqlQuery q;
    QString qstr;
    qstr = "insert into tbook (isbn, title, author, press, pub, price, copynum, stornum, classnum) values ('"+isbn+"', '"
            +title+"', '"+author+"', '"+press+"', '"+pub+"', '"+price+"', '"+copynum+"', '"+copynum+"', '"+classnum+"')";
    qDebug() << "qstr  " << qstr;
    q.exec(qstr);
}
