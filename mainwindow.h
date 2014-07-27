#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mngbook_triggered();

    void on_mngreader_triggered();

    void on_mngsys_triggered();

    void on_pushButton_listbook_clicked();

    void on_pushButton_addbook_clicked();

    void on_pushButton_borrowbook_clicked();

    void on_pushButton_returnbook_clicked();

    void on_pushButton_listnow_clicked();

    void on_pushButton_listreader_clicked();

    void on_pushButton_addreader_clicked();

    void on_pushButton_listborrow_clicked();

    void on_pushButton_listreturn_clicked();

    void on_welcome_triggered();

    void on_pushButton_backup_clicked();

    void on_pushButton_delreader_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString strf;
};

#endif // MAINWINDOW_H
