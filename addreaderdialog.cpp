#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"
#include <QtSql>

AddreaderDialog::AddreaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddreaderDialog)
{
    ui->setupUi(this);
}

AddreaderDialog::~AddreaderDialog()
{
    delete ui;
}

void AddreaderDialog::on_buttonBox_accepted()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mbook.db");
    db.open();
    QString cardid = ui->lineEdit_cardid->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString name = ui->lineEdit_name->text();
    QString sex = ui->lineEdit_sex->text();
    QString birth = ui->lineEdit_birth->text();
    QString major = ui->lineEdit_major->text();
    QSqlQuery q;
    QString qstr;
    qstr = "insert into treader (cardid, pwd, name, sex, birth, major) values ('"+cardid+"', '"+pwd+"', '"+name+"', '"+sex+"', '"+birth+"', '"+major+"')";
    qDebug() << "qstr  " << qstr;
    q.exec(qstr);
}
