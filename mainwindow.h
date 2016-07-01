#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QMap>
#include <QDebug>
#include "mslotitem.h"
#include "serversetform.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "globalheader.h"
#include <QSqlError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMap<QString,mslotitem *> *itemmap;
    void mainwidgetinit();
    QSqlDatabase mdb;
    QSqlDatabase litedb;
    Serversetform *serversetform;
    int cellcurrentrow,cellcurrentcolumn;
    void litesql_init();
    void remotesql_connect();
    void remotesql_init();
    void remote_init_read_machineinfo();
    void closeEvent(QCloseEvent *event);


    ~MainWindow();

private slots:


    void on_ipadderbtn_clicked();

    void on_actionStart_triggered();

    void on_actionSetting_triggered();

    void on_deletebtn_clicked();

    void on_maintablewidget_cellClicked(int row, int column);

    void on_serverstartbtn_clicked();

    void on_serverstopbtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
