#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":img/app"));
    LoginDialog dlg;
    if(dlg.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
