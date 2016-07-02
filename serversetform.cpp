#include "serversetform.h"
#include "ui_serversetform.h"

Serversetform::Serversetform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serversetform)
{
    ui->setupUi(this);
    litedb = QSqlDatabase::database("localdb");
    QSqlQuery query1(litedb);
    query1.exec("select * from systemset;");
    query1.next();

    QString setip = query1.value("remoteserverip").toString();
    ui->remoteserverip->setText(setip);
    QString setport = query1.value("remoteserverport").toString();
    ui->remoteserverport->setText(setport);
    QString remotedbname = query1.value("remoteserverdbname").toString();
    ui->remotedbname->setText(remotedbname);
    QString remoteusername = query1.value("remoteserverusername").toString();
    ui->remoteserverusername->setText(remoteusername);
    QString remoteuserpassword = query1.value("remoteserveruserpassword").toString();
    ui->remoteserveruserpassword->setText(remoteuserpassword);
    disconnect(ui->DBtypecombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_DBtypecombo_currentIndexChanged(QString)));
    ui->DBtypecombo->addItem("MYSQL");
    ui->DBtypecombo->addItem("ODBC");
    QString remoteservertype = query1.value("remoteservertype").toString();
    ui->DBtypecombo->setCurrentText(remoteservertype);
    connect(ui->DBtypecombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_DBtypecombo_currentIndexChanged(QString)));

}

Serversetform::~Serversetform()
{
    delete ui;
}


void Serversetform::on_applybtn_clicked()
{
    QSqlQuery query2(litedb);
    QString str = QString("update systemset set remoteserverip = \'%1\';").arg(ui->remoteserverip->text());
    query2.exec(str);
    str =  QString("update systemset set remoteserverport = \'%1\';").arg(ui->remoteserverport->text());
    query2.exec(str);
    str = QString("update systemset set remoteserverdbname = \'%1\';").arg(ui->remotedbname->text());
    query2.exec(str);
    str = QString("update systemset set remoteserverusername = \'%1\';").arg(ui->remoteserverusername->text());
    query2.exec(str);
    str = QString("update systemset set remoteserveruserpassword = \'%1\';").arg(ui->remoteserveruserpassword->text());
    query2.exec(str);

    close();


}

void Serversetform::on_DBtypecombo_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query2(litedb);
    QString str = QString("update systemset set remoteservertype = \'%1\';").arg(arg1);
    query2.exec(str);
}
