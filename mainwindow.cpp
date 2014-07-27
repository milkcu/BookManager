#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QMessageBox>
#include "addbookdialog.h"
#include "addreaderdialog.h"
#include "borrowbookdialog.h"
#include "returnbookdialog.h"
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QtWebKitWidgets/QWebView>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height()); // 禁止改变窗口大小
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mbook.db");
    db.open();
    ui->groupBox_book->setVisible(false);
    ui->groupBox_reader->setVisible(false);
    ui->groupBox_sys->setVisible(false);
    ui->tableWidget->setVisible(false);
    ui->webView->setVisible(true);
    ui->widget_welcome->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

void MainWindow::on_mngbook_triggered()
{
    ui->webView->setVisible(false);
    ui->tableWidget->setVisible(true);
    ui->groupBox_reader->setHidden(true);
    ui->groupBox_book->setVisible(true);
    ui->groupBox_sys->setVisible(false);
    ui->widget_welcome->setVisible(false);
    this->on_pushButton_listbook_clicked();
}

void MainWindow::on_mngreader_triggered()
{
    ui->webView->setVisible(false);
    ui->tableWidget->setVisible(true);
    ui->groupBox_book->setHidden(true);
    ui->groupBox_sys->setVisible(false);
    ui->groupBox_reader->setVisible(true);
    ui->widget_welcome->setVisible(false);
    this->on_pushButton_listreader_clicked();
}

void MainWindow::on_welcome_triggered()
{
    ui->webView->setVisible(true);
    ui->tableWidget->setVisible(false);
    ui->groupBox_book->setVisible(false);
    ui->groupBox_sys->setVisible(false);
    ui->groupBox_reader->setVisible(false);
    ui->widget_welcome->setVisible(true);
}

void MainWindow::on_mngsys_triggered()
{
    ui->webView->setVisible(false);
    ui->tableWidget->setVisible(true);
    ui->groupBox_book->setHidden(true);
    ui->groupBox_reader->setVisible(false);
    ui->groupBox_sys->setVisible(true);
    ui->widget_welcome->setVisible(false);
    this->on_pushButton_listborrow_clicked();
}

void MainWindow::on_pushButton_listbook_clicked()
{
    QTableWidget * tw = ui->tableWidget;
    tw->setColumnCount(10);
    QStringList header;
    header << strf.fromLocal8Bit("图书编号");
    header << strf.fromLocal8Bit("ISBN");
    header << strf.fromLocal8Bit("书名");
    header << strf.fromLocal8Bit("作译者");
    header << strf.fromLocal8Bit("出版社");
    header << strf.fromLocal8Bit("出版年月");
    header << strf.fromLocal8Bit("价格");
    header << strf.fromLocal8Bit("复本量");
    header << strf.fromLocal8Bit("库存量");
    header << strf.fromLocal8Bit("分类号");
    tw->setHorizontalHeaderLabels(header);
    // show the book list
    QSqlQuery q;
    q.exec("select count(*) from tbook");
    q.next();
    int rowcnt = q.value(0).toInt();
    tw->setRowCount(rowcnt);
    q.exec("select bid,isbn,title,author,press,pub,price,copynum,stornum,classnum from tbook");
    int cnt = 0;
    while(q.next()) {
        for(int i = 0; i < 10; i++) {
            QString qresult = q.value(i).toString();
            tw->setItem(cnt, i, new QTableWidgetItem(qresult));
        }
        cnt++;
    }
}

void MainWindow::on_pushButton_addbook_clicked()
{
    AddbookDialog dlg;
    dlg.exec();
}

void MainWindow::on_pushButton_borrowbook_clicked()
{
    if(ui->tableWidget->currentRow() < 0) {
        QMessageBox::information(NULL, strf.fromLocal8Bit("提示"), strf.fromLocal8Bit("请先选中将要借阅的书籍！"));
        return;
    }
    QTableWidgetItem * item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    QString bid = item->text();
    QSqlQuery q;
    q.exec("select stornum from tbook where bid = '"+bid+"'");
    q.next();
    int stornum_ = q.value(0).toInt();
    if(stornum_ <= 0) {
        QMessageBox::information(NULL, strf.fromLocal8Bit("错误"), strf.fromLocal8Bit("当前书籍库存不足！"));
        return;
    } else {
        BorrowbookDialog dlg;
        if(dlg.exec() == QDialog::Accepted) {
            // borrow book
            QString rid = dlg.get_rid();
            q.exec("select count(*) from tnow where bid = '"+bid+"' and rid = '"+rid+"'");
            q.next();
            if(q.value(0).toInt() != 0) {
                QMessageBox::information(NULL, strf.fromLocal8Bit("警告"), strf.fromLocal8Bit("您已借阅该图书，每人限借一本！"));
                return;
            }
            qDebug() << "rid " << rid;
            qDebug() << "bid " << bid;
            QSqlQuery q;
            // stornum - 1
            q.exec("select stornum from tbook where bid = '"+bid+"'");
            q.next();
            stornum_--;
            QString stornum = QString::number(stornum_);
            q.exec("update tbook set stornum = '"+stornum+"' where bid = '"+bid+"'");
            qDebug() << "stornum " << "update tbook set stornum = '"+stornum+"' where bid = '"+bid+"'";
            // tnow add
            QDate date = date.currentDate();
            QString btime = date.toString("yyyy-MM-dd");
            q.exec("insert into tnow (bid, rid, btime) values ('"+bid+"', '"+rid+"', '"+btime+"')");
            qDebug() << "tnow " << "insert into tnow (bid, rid, btime) values ('"+bid+"', '"+rid+"', '"+btime+"')";
            // tborrow add
            q.exec("insert into tborrow (bid, rid, btime) values ('"+bid+"', '"+rid+"', '"+btime+"')");
            qDebug() << "tborrow " << "insert into tborrow (bid, rid, btime) values ('"+bid+"', '"+rid+"', '"+btime+"')";
        }
    }
}

void MainWindow::on_pushButton_returnbook_clicked()
{
    ReturnbookDialog dlg;
    if(dlg.exec() == QDialog::Accepted) {
        QString bid = dlg.get_bid();
        QString rid = dlg.get_rid();
        QSqlQuery q;
        // stornum + 1
        q.exec("select stornum from tbook where bid = '"+bid+"'");
        q.next();
        int stornum_ = q.value(0).toInt();
        stornum_++;
        QString stornum = QString::number(stornum_);
        q.exec("update tbook set stornum = '"+stornum+"' where bid = '"+bid+"'");
        // tnow del
        QDate date = date.currentDate();
        QString rtime = date.toString("yyyy-MM-dd");
        q.exec("delete from tnow where bid = '"+bid+"' and rid = '"+rid+"'");
        // treturn add
        q.exec("insert into treturn (bid, rid, rtime) values ('"+bid+"', '"+rid+"', '"+rtime+"')");
    }
}

void MainWindow::on_pushButton_listnow_clicked()
{
    QTableWidget * tw = ui->tableWidget;
    tw->setColumnCount(5);
    QStringList header;
    header << strf.fromLocal8Bit("图书编号");
    header << strf.fromLocal8Bit("图书名称");
    header << strf.fromLocal8Bit("读者编号");
    header << strf.fromLocal8Bit("读者姓名");
    header << strf.fromLocal8Bit("借出时间");
    tw->setHorizontalHeaderLabels(header);
    // show the book list
    QSqlQuery q;
    q.exec("select count(*) from tnow");
    q.next();
    int rowcnt = q.value(0).toInt();
    tw->setRowCount(rowcnt);
    q.exec("select bid,rid,btime from tnow");
    int cnt = 0;
    while(q.next()) {
        QSqlQuery q2;
        QString bid = q.value(0).toString();
        QString rid = q.value(1).toString();
        q2.exec("select title from tbook where bid = '"+bid+"'");
        q2.next();
        QString title = q2.value(0).toString();
        q2.exec("select name from treader where rid = '"+rid+"'");
        q2.next();
        QString name = q2.value(0).toString();
        tw->setItem(cnt, 0, new QTableWidgetItem(bid));
        tw->setItem(cnt, 1, new QTableWidgetItem(title));
        tw->setItem(cnt, 2, new QTableWidgetItem(rid));
        tw->setItem(cnt, 3, new QTableWidgetItem(name));
        tw->setItem(cnt, 4, new QTableWidgetItem(q.value(2).toString()));
        cnt++;
    }
}

void MainWindow::on_pushButton_listreader_clicked()
{
    QTableWidget * tw = ui->tableWidget;
    tw->setColumnCount(8);
    QStringList header;
    header << strf.fromLocal8Bit("读者编号");
    header << strf.fromLocal8Bit("借书证号");
    header << strf.fromLocal8Bit("密码");
    header << strf.fromLocal8Bit("姓名");
    header << strf.fromLocal8Bit("性别");
    header << strf.fromLocal8Bit("出生时间");
    header << strf.fromLocal8Bit("专业");
    header << strf.fromLocal8Bit("借书量");
    tw->setHorizontalHeaderLabels(header);
    // show the book list
    QSqlQuery q;
    q.exec("select count(*) from treader");
    q.next();
    int rowcnt = q.value(0).toInt();
    tw->setRowCount(rowcnt);
    q.exec("select rid,cardid,pwd,name,sex,birth,major from treader");
    int cnt = 0;
    while(q.next()) {
        for(int i = 0; i < 7; i++) {
            QString qresult = q.value(i).toString();
            tw->setItem(cnt, i, new QTableWidgetItem(qresult));
        }
        QSqlQuery q2;
        q2.exec("select count(*) from tnow where rid = '"+q.value(0).toString()+"'");
        q2.next();
        tw->setItem(cnt, 7, new QTableWidgetItem(q2.value(0).toString()));
        cnt++;
    }
}

void MainWindow::on_pushButton_addreader_clicked()
{
    AddreaderDialog dlg;
    dlg.exec();
}

void MainWindow::on_pushButton_listborrow_clicked()
{
    QTableWidget * tw = ui->tableWidget;
    tw->setColumnCount(5);
    QStringList header;
    header << strf.fromLocal8Bit("图书编号");
    header << strf.fromLocal8Bit("图书名称");
    header << strf.fromLocal8Bit("读者编号");
    header << strf.fromLocal8Bit("读者姓名");
    header << strf.fromLocal8Bit("借出时间");
    tw->setHorizontalHeaderLabels(header);
    // show the book list
    QSqlQuery q;
    q.exec("select count(*) from tborrow");
    q.next();
    int rowcnt = q.value(0).toInt();
    tw->setRowCount(rowcnt);
    q.exec("select bid,rid,btime from tborrow");
    int cnt = 0;
    while(q.next()) {
        QSqlQuery q2;
        QString bid = q.value(0).toString();
        QString rid = q.value(1).toString();
        q2.exec("select title from tbook where bid = '"+bid+"'");
        q2.next();
        QString title = q2.value(0).toString();
        q2.exec("select name from treader where rid = '"+rid+"'");
        q2.next();
        QString name = q2.value(0).toString();
        tw->setItem(cnt, 0, new QTableWidgetItem(bid));
        tw->setItem(cnt, 1, new QTableWidgetItem(title));
        tw->setItem(cnt, 2, new QTableWidgetItem(rid));
        tw->setItem(cnt, 3, new QTableWidgetItem(name));
        tw->setItem(cnt, 4, new QTableWidgetItem(q.value(2).toString()));
        cnt++;
    }
}

void MainWindow::on_pushButton_listreturn_clicked()
{
    QTableWidget * tw = ui->tableWidget;
    tw->setColumnCount(5);
    QStringList header;
    header << strf.fromLocal8Bit("图书编号");
    header << strf.fromLocal8Bit("图书名称");
    header << strf.fromLocal8Bit("读者编号");
    header << strf.fromLocal8Bit("读者姓名");
    header << strf.fromLocal8Bit("归还时间");
    tw->setHorizontalHeaderLabels(header);
    // show the book list
    QSqlQuery q;
    q.exec("select count(*) from treturn");
    q.next();
    int rowcnt = q.value(0).toInt();
    tw->setRowCount(rowcnt);
    q.exec("select bid,rid,rtime from treturn");
    int cnt = 0;
    while(q.next()) {
        QSqlQuery q2;
        QString bid = q.value(0).toString();
        QString rid = q.value(1).toString();
        q2.exec("select title from tbook where bid = '"+bid+"'");
        q2.next();
        QString title = q2.value(0).toString();
        q2.exec("select name from treader where rid = '"+rid+"'");
        q2.next();
        QString name = q2.value(0).toString();
        tw->setItem(cnt, 0, new QTableWidgetItem(bid));
        tw->setItem(cnt, 1, new QTableWidgetItem(title));
        tw->setItem(cnt, 2, new QTableWidgetItem(rid));
        tw->setItem(cnt, 3, new QTableWidgetItem(name));
        tw->setItem(cnt, 4, new QTableWidgetItem(q.value(2).toString()));
        cnt++;
    }
}

void MainWindow::on_pushButton_backup_clicked()
{
    //db.close();
    QString filename = QFileDialog::getSaveFileName(this, strf.fromLocal8Bit("数据备份"), tr("backup.db"));
    qDebug() << "filename " << filename;
    QFile::copy("mbook.db", filename);
    //db.open();
}

void MainWindow::on_pushButton_delreader_clicked()
{
    if(ui->tableWidget->currentRow() < 0) {
        QMessageBox::information(NULL, strf.fromLocal8Bit("提示"), strf.fromLocal8Bit("请先选中将要删除的读者！"));
        return;
    }
    QTableWidgetItem * item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    QString rid = item->text();
    QSqlQuery q;
    q.exec("select count(*) from tnow where rid = '"+rid+"'");
    q.next();
    if(q.value(0).toInt() > 0) {
        QMessageBox::information(NULL, strf.fromLocal8Bit("错误"), strf.fromLocal8Bit("当前读者所借图书尚未完全归还！"));
        return;
    } else {
        q.exec("delete from treader where rid = '"+rid+"'");
        QMessageBox::information(NULL, strf.fromLocal8Bit("成功"), strf.fromLocal8Bit("所选读者删除成功！"));
    }
}
