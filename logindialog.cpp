#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    QString user = ui->lineEdit_user->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString strf;
    if(user != "admin" && pwd != "admin") {
        if(user == "11037" && pwd == "11037") {
            QMessageBox::information(NULL, strf.fromLocal8Bit("欢迎"), strf.fromLocal8Bit("欢迎徐老师！"));
            accept();
        } else {
            QMessageBox::information(NULL, strf.fromLocal8Bit("登录失败"), strf.fromLocal8Bit("用户名或密码错误！"));
        }
    } else {
        accept();
    }
}
