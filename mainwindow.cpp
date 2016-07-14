#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    litesql_init();

    serversetform = new Serversetform();

    remotesql_connect();

    remotesql_init();

    mainwidgetinit();

    remote_init_read_machineinfo();

    cellcurrentrow = -1;
    cellcurrentcolumn = -1;

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mainwidgetinit(){
    ui->maintablewidget->setHorizontalHeaderItem(0,new QTableWidgetItem(QIcon(":/icon/icon/link.png"),"connect"));
    ui->maintablewidget->setHorizontalHeaderItem(1,new QTableWidgetItem(QIcon(":/icon/icon/clock.png"),"status"));
    ui->maintablewidget->setHorizontalHeaderItem(2,new QTableWidgetItem(QIcon(":/icon/icon/ipaddress.png"),"ip"));
    ui->maintablewidget->setHorizontalHeaderItem(3,new QTableWidgetItem(QIcon(":/icon/icon/screen.png"),"name"));
    ui->maintablewidget->setHorizontalHeaderItem(4,new QTableWidgetItem(QIcon(":/icon/icon/sewing-machine.png"),"type"));
    ui->maintablewidget->setHorizontalHeaderItem(5,new QTableWidgetItem(QIcon(":/icon/icon/settings.png"),"setup"));
    itemmap = new QMap<QString,mslotitem *>;

}
void MainWindow::remote_init_read_machineinfo(){
    QSqlQuery mysqlquery1(mdb);
    QString query1 = QString("Select machine_name,"
                             "ipaddress,"
                             "ITEMCONNECT,"
                             "ITEMSTATUS,"
                             "ITEMTYPE "
                             "from Systeminfo");

    mysqlquery1.exec(query1);
    while(mysqlquery1.next()){
        ui->maintablewidget->insertRow(ui->maintablewidget->rowCount());
        int countnumber = ui->maintablewidget->rowCount() - 1;
         mslotitem *temp_item = new mslotitem(mysqlquery1.value("ipaddress").toString(),
                                              mysqlquery1.value("machine_name").toString());

         itemmap->insert(temp_item->machinenametext,temp_item);
         ui->maintablewidget->setCellWidget(countnumber,ITEMCONNECT,temp_item->connectlabel);
         ui->maintablewidget->setCellWidget(countnumber,ITEMSTATUS,temp_item->status);
         ui->maintablewidget->setCellWidget(countnumber,ITEMIP,temp_item->ip);
         ui->maintablewidget->setCellWidget(countnumber,ITEMNAME,temp_item->machinename);
         ui->maintablewidget->setCellWidget(countnumber,ITEMTYPE,temp_item->type);
         ui->maintablewidget->setCellWidget(countnumber,ITEMSETUP,temp_item->setupbtn);
    }
}

void MainWindow::on_ipadderbtn_clicked()
{

    QSqlQuery mysqlquery1(mdb);
    if(ui->QLaddip->text().compare("...") == 0){
        ui->logtext->append(tr("ip 가입력되지 않았습니다. "));
        return;
    }else if(ui->QLaddmachinename->text().trimmed().compare("")==0 ){
        ui->logtext->append(tr("기계이이름  등록되지 않았습니다."));
        return;
    }
    QString quertstr1 = QString("select machine_name from Systeminfo "
                                "where machine_name = '%1'").arg(ui->QLaddmachinename->text());
    mysqlquery1.exec(quertstr1);
    //검색결과가 있으면 해당중복의로판단
    if(mysqlquery1.next()){
        ui->logtext->append(tr("이미 등록된 기계이름입니다."));
        return;
    }
    ui->maintablewidget->insertRow(ui->maintablewidget->rowCount());
    int countnumber = ui->maintablewidget->rowCount() - 1;
    mslotitem *temp_item = new mslotitem(ui->QLaddip->text(),ui->QLaddmachinename->text());
    itemmap->insert(temp_item->machinenametext,temp_item);
    ui->maintablewidget->setCellWidget(countnumber,ITEMCONNECT,temp_item->connectlabel);
    ui->maintablewidget->setCellWidget(countnumber,ITEMSTATUS,temp_item->status);
    ui->maintablewidget->setCellWidget(countnumber,ITEMIP,temp_item->ip);
    ui->maintablewidget->setCellWidget(countnumber,ITEMNAME,temp_item->machinename);
    ui->maintablewidget->setCellWidget(countnumber,ITEMTYPE,temp_item->type);
    ui->maintablewidget->setCellWidget(countnumber,ITEMSETUP,temp_item->setupbtn);
}

void MainWindow::on_actionStart_triggered()
{

}

void MainWindow::on_actionSetting_triggered()
{
    serversetform->show();
}
void MainWindow::litesql_init(){
    litedb = QSqlDatabase::addDatabase("QSQLITE","localdb");
    litedb.setDatabaseName("local.db");
    if(!litedb.open()){
        qDebug()<<"localdb open fail";
        ui->logtext->append("localdb open fail");
    }else {
        ui->logtext->append("localdb open");
    }
    QSqlQuery litequery1(litedb);
    litequery1.exec("CREATE TABLE  IF NOT EXISTS systemset (remoteserverip TEXT,"
                    "remoteserverport TEXT,"
                    "remoteserverdbname TEXT,"
                    "version INTEGER,"
                    "remoteserverusername TEXT,"
                    "remoteserveruserpassword TEXT,"
                    "remoteservertype TEXT"
                    ");");
    //만약조건이없다면 업데이트
    litequery1.exec("insert into systemset(remoteserverip,"
                    "remoteserverport,"
                    "remoteserverdbname,"
                    "version,"
                    "remoteserverusername,"
                    "remoteserveruserpassword,"
                    "remoteservertype) "
                    "select \'127.0.0.1\',"
                    "\'3306\',"
                    "\'QCproject\',"
                    "1,"
                    "\'QCmen\',"
                    "\'1234\',"
                    "\'MYSQL\' "
                    "where not exists(select * from systemset);");

}
void MainWindow::remotesql_connect(){
    QSqlQuery litequery1(litedb);
    litequery1.exec("select * from systemset;");
    litequery1.next();
    if(litequery1.value("remoteservertype").toString().compare("MYSQL")==0){
        mdb = QSqlDatabase::addDatabase("QMYSQL","remotedb");
    }else if(litequery1.value("remoteservertype").toString().compare("ODBC")==0){
        mdb = QSqlDatabase::addDatabase("QODBC","remotedb");
    }
    mdb.setHostName(litequery1.value("remoteserverip").toString());
    mdb.setDatabaseName(litequery1.value("remoteserverdbname").toString());
    mdb.setPort(litequery1.value("remoteserverport").toInt());
    mdb.setUserName(litequery1.value("remoteserverusername").toString());
    mdb.setPassword(litequery1.value("remoteserveruserpassword").toString());

    if(!mdb.open()){
        qDebug()<<"DB not open";
        ui->logtext->append("DB not open");
    }else {
        ui->logtext->append("remtoe DB open");
    }
    qDebug()<<mdb.lastError().text();
}

void MainWindow::remotesql_init(){
    QSqlQuery mysqlquery1(mdb);
    QSqlQuery litequery1(litedb);
    litequery1.exec("select * from systemset;");
    litequery1.next();
    int type = 0;
    if(litequery1.value("remoteservertype").toString().compare("MYSQL")==0){
        type =MYSQL;
    }else if(litequery1.value("remoteservertype").toString().compare("ODBC")==0){
        type =ODBC;
    }
    if(type == MYSQL){
    mysqlquery1.exec("CREATE TABLE `DBvsersion` ("
                         "`id` INT(11) NOT NULL DEFAULT '1',"
                         "`systeminfoversion` INT(11) NULL DEFAULT NULL,"
                         "`temp_tableversion` INT(11) NULL DEFAULT NULL,"
                         "`mold_infoversion` INT(11) NULL DEFAULT NULL,"
                         "`DBversion` INT(11) NULL DEFAULT NULL,"
                         "PRIMARY KEY (`id`)"
                     ")"
                     "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                );
    }else if(type == ODBC){
    mysqlquery1.exec("CREATE TABLE DBvsersion ("
                             "id INT NOT NULL DEFAULT '1',"
                             "systeminfoversion INT NULL DEFAULT NULL,"
                             "temp_tableversion INT NULL DEFAULT NULL,"
                             "mold_infoversion INT NULL DEFAULT NULL,"
                             "DBversion INT NULL DEFAULT NULL,"
                             "PRIMARY KEY (id)"
                         ")"
                         ""
                         ""
                         ";"
                    );

    }
    if(type == MYSQL){
    mysqlquery1.exec("CREATE TABLE `Systeminfo` ("
                         "`machine_name` VARCHAR(50) NULL DEFAULT NULL COMMENT '',"
                         "`ipaddress` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`ITEMCONNECT` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`ITEMSTATUS` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`ITEMTYPE` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`mold_name` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`worker` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`item_code` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`item_name` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`orders_count` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`cycle_time` TIME NULL DEFAULT NULL COMMENT ' ',"
                         "`cabity` TEXT NULL COMMENT ' ' COLLATE 'utf8_bin',"
                         "`production_count` TEXT NULL COLLATE 'utf8_bin',"
                         "`object_count` TEXT NULL COLLATE 'utf8_bin',"
                         "`achievemen_rate` TEXT NULL COLLATE 'utf8_bin',"
                         "`good_count` TEXT NULL COLLATE 'utf8_bin',"
                         "`poor_count` TEXT NULL COLLATE 'utf8_bin',"
                         "`weight` TEXT NULL COLLATE 'utf8_bin',"
                         "`run_mode` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`warning_flag`  TEXT NULL COLLATE 'utf8_bin',"
                         "UNIQUE INDEX `machine_name` (`machine_name`)"
                     ")"
                      "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                   );
    }else if(type == ODBC){
    mysqlquery1.exec("CREATE TABLE Systeminfo ("
                             "machine_name VARCHAR(50) NULL DEFAULT NULL  ,"
                             "ipaddress TEXT NULL DEFAULT NULL  ,"
                             "ITEMCONNECT TEXT NULL DEFAULT NULL  ,"
                             "ITEMSTATUS TEXT NULL DEFAULT NULL  ,"
                             "ITEMTYPE TEXT NULL DEFAULT NULL  ,"
                             "mold_name TEXT NULL DEFAULT NULL  ,"
                             "worker TEXT NULL DEFAULT NULL  ,"
                             "item_code TEXT NULL DEFAULT NULL  ,"
                             "item_name TEXT NULL DEFAULT NULL  ,"
                             "orders_count TEXT NULL DEFAULT NULL  ,"
                             "cycle_time TIME NULL DEFAULT NULL  ,"
                             "cabity INT NOT NULL DEFAULT '0'  ,"
                             "production_count INT NOT NULL DEFAULT '0' ,"
                             "object_count INT NOT NULL DEFAULT '0' ,"
                             "achievemen_rate float  NOT NULL DEFAULT '0' ,"
                             "good_count INT NOT NULL DEFAULT '0' ,"
                             "poor_count INT NOT NULL DEFAULT '0' ,"
                             "weight float NOT NULL DEFAULT '0' ,"
                             "run_mode TEXT NULL DEFAULT NULL ,"
                             "warning_flag INT NOT NULL DEFAULT '0',"
                             "UNIQUE (machine_name)"
                         ")"
                          ""
                         ""
                         ";"
                       );
    }


    if(type == MYSQL){
    mysqlquery1.exec(tr("CREATE TABLE `mold_info` ("
                         "`mold_name` VARCHAR(50) NULL DEFAULT NULL COMMENT '',"
                         "`item_name` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`item_code` TEXT NULL DEFAULT NULL COMMENT '',"
                         "UNIQUE INDEX `mold_name` (`mold_name`)"
                     ")"
                    "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                  )
                );
    }else if(type == ODBC){
        mysqlquery1.exec(tr("CREATE TABLE mold_info ("
                             "mold_name VARCHAR(50) NULL DEFAULT NULL ,"
                             "item_name TEXT NULL DEFAULT NULL ,"
                             "item_code TEXT NULL DEFAULT NULL ,"
                             "UNIQUE (mold_name)"
                         ")"
                        ""
                         ";"
                      )
                    );
    }

    if(type == MYSQL){
    mysqlquery1.exec(tr("CREATE TABLE `temp_table` ("
                         "`machine_name` VARCHAR(50) NULL DEFAULT NULL,"
                        "`temp1_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp1_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp1_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp1_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp1_name` VARCHAR(50) NOT NULL DEFAULT 'temp1' COMMENT ' ' COLLATE 'utf8_bin',"
                        "`temp1_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp2_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp2_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp2_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp2_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp2_name` VARCHAR(50) NOT NULL DEFAULT 'temp2' COLLATE 'utf8_bin',"
                        "`temp2_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp3_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp3_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp3_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp3_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp3_name` VARCHAR(50) NOT NULL DEFAULT 'temp3' COLLATE 'utf8_bin',"
                        "`temp3_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp4_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp4_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp4_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp4_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp4_name` VARCHAR(50) NOT NULL DEFAULT 'temp4' COLLATE 'utf8_bin',"
                        "`temp4_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp5_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp5_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp5_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp5_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp5_name` VARCHAR(50) NOT NULL DEFAULT 'temp5' COLLATE 'utf8_bin',"
                        "`temp5_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp6_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp6_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp6_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp6_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp6_name` VARCHAR(50) NOT NULL DEFAULT 'temp6' COLLATE 'utf8_bin',"
                        "`temp6_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp7_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp7_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp7_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp7_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp7_name` VARCHAR(50) NOT NULL DEFAULT 'temp7' COLLATE 'utf8_bin',"
                        "`temp7_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp8_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp8_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp8_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp8_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp8_name` VARCHAR(50) NOT NULL DEFAULT 'temp8' COLLATE 'utf8_bin',"
                        "`temp8_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp9_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp9_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp9_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp9_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp9_name` VARCHAR(50) NOT NULL DEFAULT 'temp9' COLLATE 'utf8_bin',"
                        "`temp9_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp10_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp10_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp10_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp10_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp10_name` VARCHAR(50) NOT NULL DEFAULT 'temp10' COLLATE 'utf8_bin',"
                        "`temp10_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp11_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp11_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp11_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp11_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp11_name` VARCHAR(50) NOT NULL DEFAULT 'temp11' COLLATE 'utf8_bin',"
                        "`temp11_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp12_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp12_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp12_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp12_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp12_name` VARCHAR(50) NOT NULL DEFAULT 'temp12' COLLATE 'utf8_bin',"
                        "`temp12_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp13_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp13_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp13_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp13_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp13_name` VARCHAR(50) NOT NULL DEFAULT 'temp13' COLLATE 'utf8_bin',"
                        "`temp13_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp14_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp14_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp14_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp14_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp14_name` VARCHAR(50) NOT NULL DEFAULT 'temp14' COLLATE 'utf8_bin',"
                        "`temp14_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp15_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp15_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp15_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp15_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp15_name` VARCHAR(50) NOT NULL DEFAULT 'temp15' COLLATE 'utf8_bin',"
                        "`temp15_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp16_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp16_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp16_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp16_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp16_name` VARCHAR(50) NOT NULL DEFAULT 'temp16' COLLATE 'utf8_bin',"
                        "`temp16_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp17_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp17_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp17_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp17_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp17_name` VARCHAR(50) NOT NULL DEFAULT 'temp17' COLLATE 'utf8_bin',"
                        "`temp17_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp18_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp18_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp18_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp18_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp18_name` VARCHAR(50) NOT NULL DEFAULT 'temp18' COLLATE 'utf8_bin',"
                        "`temp18_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp19_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp19_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp19_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp19_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp19_name` VARCHAR(50) NOT NULL DEFAULT 'temp19' COLLATE 'utf8_bin',"
                        "`temp19_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp20_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp20_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp20_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp20_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp20_name` VARCHAR(50) NOT NULL DEFAULT 'temp20' COLLATE 'utf8_bin',"
                        "`temp20_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp21_set` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp21_up` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp21_down` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp21_real` TEXT NULL COLLATE 'utf8_bin',"
                        "`temp21_name` VARCHAR(50) NOT NULL DEFAULT 'temp21' COLLATE 'utf8_bin',"
                        "`temp21_onoff` TEXT NULL COLLATE 'utf8_bin',"
                        "`modift_version` TEXT NULL COLLATE 'utf8_bin',"
                         "UNIQUE INDEX `machine_name` (`machine_name`)"
                     ")"
                    "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                ));
    }else if(type == ODBC){
        mysqlquery1.exec(tr("CREATE TABLE temp_table ("
                             "machine_name VARCHAR(50) NULL DEFAULT NULL,"
                             "temp1_set float NOT NULL DEFAULT '0' ,"
                             "temp1_up float NOT NULL DEFAULT '0' ,"
                             "temp1_down float NOT NULL DEFAULT '0' ,"
                             "temp1_real float NOT NULL DEFAULT '0' ,"
                             "temp1_name VARCHAR(50) NOT NULL DEFAULT 'temp1',"
                             "temp1_onoff INT NOT NULL DEFAULT '0',"
                             "temp2_set float NOT NULL DEFAULT '0' ,"
                             "temp2_up float NOT NULL DEFAULT '0' ,"
                             "temp2_down float NOT NULL DEFAULT '0' ,"
                             "temp2_real float NOT NULL DEFAULT '0' ,"
                             "temp2_name VARCHAR(50) NOT NULL DEFAULT 'temp2' ,"
                             "temp2_onoff INT NOT NULL DEFAULT '0',"
                             "temp3_set float NOT NULL DEFAULT '0' ,"
                             "temp3_up float NOT NULL DEFAULT '0' ,"
                             "temp3_down float NOT NULL DEFAULT '0' ,"
                             "temp3_real float NOT NULL DEFAULT '0' ,"
                             "temp3_name VARCHAR(50) NOT NULL DEFAULT 'temp3' ,"
                             "temp3_onoff INT NOT NULL DEFAULT '0',"
                             "temp4_set float NOT NULL DEFAULT '0' ,"
                             "temp4_up float NOT NULL DEFAULT '0' ,"
                             "temp4_down float NOT NULL DEFAULT '0' ,"
                             "temp4_real float NOT NULL DEFAULT '0' ,"
                             "temp4_name VARCHAR(50) NOT NULL DEFAULT 'temp4' ,"
                             "temp4_onoff INT NOT NULL DEFAULT '0',"
                             "temp5_set float NOT NULL DEFAULT '0' ,"
                             "temp5_up float NOT NULL DEFAULT '0' ,"
                             "temp5_down float NOT NULL DEFAULT '0' ,"
                             "temp5_real float NOT NULL DEFAULT '0' ,"
                             "temp5_name VARCHAR(50) NOT NULL DEFAULT 'temp5' ,"
                             "temp5_onoff INT NOT NULL DEFAULT '0',"
                             "temp6_set float NOT NULL DEFAULT '0' ,"
                             "temp6_up float NOT NULL DEFAULT '0' ,"
                             "temp6_down float NOT NULL DEFAULT '0' ,"
                             "temp6_real float NOT NULL DEFAULT '0' ,"
                             "temp6_name VARCHAR(50) NOT NULL DEFAULT 'temp6' ,"
                             "temp6_onoff INT NOT NULL DEFAULT '0',"
                             "temp7_set float NOT NULL DEFAULT '0' ,"
                             "temp7_up float NOT NULL DEFAULT '0' ,"
                             "temp7_down float NOT NULL DEFAULT '0' ,"
                             "temp7_real float NOT NULL DEFAULT '0' ,"
                             "temp7_name VARCHAR(50) NOT NULL DEFAULT 'temp7' ,"
                             "temp7_onoff INT NOT NULL DEFAULT '0',"
                             "temp8_set float NOT NULL DEFAULT '0' ,"
                             "temp8_up float NOT NULL DEFAULT '0' ,"
                             "temp8_down float NOT NULL DEFAULT '0' ,"
                             "temp8_real float NOT NULL DEFAULT '0' ,"
                             "temp8_name VARCHAR(50) NOT NULL DEFAULT 'temp8' ,"
                             "temp8_onoff INT NOT NULL DEFAULT '0',"
                             "temp9_set float NOT NULL DEFAULT '0' ,"
                             "temp9_up float NOT NULL DEFAULT '0' ,"
                             "temp9_down float NOT NULL DEFAULT '0' ,"
                             "temp9_real float NOT NULL DEFAULT '0' ,"
                             "temp9_name VARCHAR(50) NOT NULL DEFAULT 'temp9' ,"
                             "temp9_onoff INT NOT NULL DEFAULT '0',"
                             "temp10_set float NOT NULL DEFAULT '0' ,"
                             "temp10_up float NOT NULL DEFAULT '0' ,"
                             "temp10_down float NOT NULL DEFAULT '0' ,"
                             "temp10_real float NOT NULL DEFAULT '0' ,"
                             "temp10_name VARCHAR(50) NOT NULL DEFAULT 'temp10' ,"
                             "temp10_onoff INT NOT NULL DEFAULT '0',"
                             "temp11_set float NOT NULL DEFAULT '0' ,"
                             "temp11_up float NOT NULL DEFAULT '0' ,"
                             "temp11_down float NOT NULL DEFAULT '0' ,"
                             "temp11_real float NOT NULL DEFAULT '0' ,"
                             "temp11_name VARCHAR(50) NOT NULL DEFAULT 'temp11' ,"
                             "temp11_onoff INT NOT NULL DEFAULT '0',"
                             "temp12_set float NOT NULL DEFAULT '0' ,"
                             "temp12_up float NOT NULL DEFAULT '0' ,"
                             "temp12_down float NOT NULL DEFAULT '0' ,"
                             "temp12_real float NOT NULL DEFAULT '0' ,"
                             "temp12_name VARCHAR(50) NOT NULL DEFAULT 'temp12' ,"
                             "temp12_onoff INT NOT NULL DEFAULT '0',"
                             "temp13_set float NOT NULL DEFAULT '0' ,"
                             "temp13_up float NOT NULL DEFAULT '0' ,"
                             "temp13_down float NOT NULL DEFAULT '0' ,"
                             "temp13_real float NOT NULL DEFAULT '0' ,"
                             "temp13_name VARCHAR(50) NOT NULL DEFAULT 'temp13' ,"
                             "temp13_onoff INT NOT NULL DEFAULT '0',"
                             "temp14_set float NOT NULL DEFAULT '0' ,"
                             "temp14_up float NOT NULL DEFAULT '0' ,"
                             "temp14_down float NOT NULL DEFAULT '0' ,"
                             "temp14_real float NOT NULL DEFAULT '0' ,"
                             "temp14_name VARCHAR(50) NOT NULL DEFAULT 'temp14',"
                             "temp14_onoff INT NOT NULL DEFAULT '0',"
                             "temp15_set float NOT NULL DEFAULT '0' ,"
                             "temp15_up float NOT NULL DEFAULT '0' ,"
                             "temp15_down float NOT NULL DEFAULT '0' ,"
                             "temp15_real float NOT NULL DEFAULT '0' ,"
                             "temp15_name VARCHAR(50) NOT NULL DEFAULT 'temp15',"
                             "temp15_onoff INT NOT NULL DEFAULT '0',"
                             "temp16_set float NOT NULL DEFAULT '0' ,"
                             "temp16_up float NOT NULL DEFAULT '0' ,"
                             "temp16_down float NOT NULL DEFAULT '0' ,"
                             "temp16_real float NOT NULL DEFAULT '0' ,"
                             "temp16_name VARCHAR(50) NOT NULL DEFAULT 'temp16' ,"
                             "temp16_onoff INT NOT NULL DEFAULT '0',"
                             "temp17_set float NOT NULL DEFAULT '0' ,"
                             "temp17_up float NOT NULL DEFAULT '0' ,"
                             "temp17_down float NOT NULL DEFAULT '0' ,"
                             "temp17_real float NOT NULL DEFAULT '0' ,"
                             "temp17_name VARCHAR(50) NOT NULL DEFAULT 'temp17' ,"
                             "temp17_onoff INT NOT NULL DEFAULT '0',"
                             "temp18_set float NOT NULL DEFAULT '0' ,"
                             "temp18_up float NOT NULL DEFAULT '0' ,"
                             "temp18_down float NOT NULL DEFAULT '0' ,"
                             "temp18_real float NOT NULL DEFAULT '0' ,"
                             "temp18_name VARCHAR(50) NOT NULL DEFAULT 'temp18' ,"
                             "temp18_onoff INT NOT NULL DEFAULT '0',"
                             "temp19_set float NOT NULL DEFAULT '0' ,"
                             "temp19_up float NOT NULL DEFAULT '0' ,"
                             "temp19_down float NOT NULL DEFAULT '0' ,"
                             "temp19_real float NOT NULL DEFAULT '0' ,"
                             "temp19_name VARCHAR(50) NOT NULL DEFAULT 'temp19' ,"
                             "temp19_onoff INT NOT NULL DEFAULT '0',"
                             "temp20_set float NOT NULL DEFAULT '0' ,"
                             "temp20_up float NOT NULL DEFAULT '0' ,"
                             "temp20_down float NOT NULL DEFAULT '0' ,"
                             "temp20_real float NOT NULL DEFAULT '0' ,"
                             "temp20_name VARCHAR(50) NOT NULL DEFAULT 'temp20' ,"
                             "temp20_onoff INT NOT NULL DEFAULT '0',"
                             "temp21_set float NOT NULL DEFAULT '0' ,"
                             "temp21_up float NOT NULL DEFAULT '0' ,"
                             "temp21_down float NOT NULL DEFAULT '0' ,"
                             "emp21_real float NOT NULL DEFAULT '0' ,"
                             "temp21_name VARCHAR(50) NOT NULL DEFAULT 'temp21' ,"
                             "temp21_onoff INT NOT NULL DEFAULT '0',"
                             "modift_version INT NOT NULL DEFAULT '0' ,"
                             "UNIQUE (machine_name)"
                         ")"

                         ";"
                    ));

    }

   if(type == MYSQL){
    QString strquery = QString("INSERT INTO DBvsersion ("
                               "id,"
                               "systeminfoversion,"
                               "temp_tableversion,"
                               "mold_infoversion,"
                               "DBversion"
                               ") values ("
                               "1,"
                               "%1,"    //systeminfoversion
                               "%2,"    //temp_tableversion
                               "%3,"    //mold_infoversion
                               "%4"    //DBversion
                               ") ON DUPLICATE KEY UPDATE "
                               "systeminfoversion = %1,"
                               "temp_tableversion = %2,"
                               "mold_infoversion = %3,"
                               "DBversion = %4"
                               ";"
                          ).arg(SYSTEMINFOVERSION)
                           .arg(TEMPTABLEVERSION)
                           .arg(MOLDINFOVERSION)
                           .arg(DBVERSION);
        mysqlquery1.exec(strquery);
   }else if(type == ODBC){
       QString strquery = QString("UPDATE DBvsersion set "
                                  "systeminfoversion = %1,"
                                  "temp_tableversion = %2,"
                                  "mold_infoversion = %3,"
                                  "DBversion = %4 "
                                  "where id = 1 "
                                  "IF @@ROWCOUNT=0 INSERT INTO DBvsersion("
                                  "id,"
                                  "systeminfoversion,"
                                  "temp_tableversion,"
                                  "mold_infoversion,"
                                  "DBversion) "
                                  "values(1,%1,%2,%3,%4)"
                                  ).arg(SYSTEMINFOVERSION)
                                    .arg(TEMPTABLEVERSION)
                                    .arg(MOLDINFOVERSION)
                                    .arg(DBVERSION);
           mysqlquery1.exec(strquery);
   }

   if(type == MYSQL){
    mysqlquery1.exec("CREATE TABLE `works` ("
                         "`number` VARCHAR(50) NOT NULL DEFAULT '0' COLLATE 'utf8_bin',"
                         "`name` VARCHAR(50) NULL DEFAULT NULL COLLATE 'utf8_bin',"
                         "`time` VARCHAR(50) NULL DEFAULT NULL COLLATE 'utf8_bin',"
                         "UNIQUE INDEX `number` (`number`)"
                     ")"
                     "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                    );
   }else if(type == ODBC){
       mysqlquery1.exec("CREATE TABLE works ("
                            "number VARCHAR(50) NOT NULL DEFAULT '0' ,"
                            "name VARCHAR(50) NULL DEFAULT NULL ,"
                            "time VARCHAR(50) NULL DEFAULT NULL ,"
                            "UNIQUE (number)"
                        ")"
                        ";"
                       );

   }

    if(type == MYSQL){
    mysqlquery1.exec("CREATE TABLE IF NOT EXISTS `Recipe_Info` ("
                                      "`machine_name` VARCHAR(50) NOT NULL,"
                                         "`injstep` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdstep` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_6` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_7` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_8` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_9` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injspd_10` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_6` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_7` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_8` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_9` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injprs_10` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_6` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_7` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_8` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_9` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injpos_10` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdspd_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdspd_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdspd_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdspd_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdspd_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdprs_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdprs_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdprs_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdprs_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdprs_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdtime_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdtime_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdtime_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdtime_4` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`holdtime_5` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgspd_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgspd_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgspd_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgbps_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgbps_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgbps_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgpos_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgpos_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgpos_3` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`suckbspd_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`suckbspd_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`suckbpos_1` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`suckbpos_2` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`sovpos` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`sovprs` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injtime` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`injdeltime` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`cooltime` TEXT NOT NULL COLLATE 'utf8_bin',"
                                         "`chgdeltime` TEXT NOT NULL COLLATE 'utf8_bin',"
                                      "UNIQUE INDEX `machine_name` (`machine_name`)"
                                  ")"
                                  "COLLATE='utf8_bin'"
                                  "ENGINE=InnoDB"
                                  ";");
    }else if(type == ODBC){
        mysqlquery1.exec("CREATE TABLE Recipe_Info ("
                                          "machine_name VARCHAR(50) NOT NULL,"
                                          "injstep SMALLINT NOT NULL DEFAULT '0',"
                                          "holdstep SMALLINT NOT NULL DEFAULT '0',"
                                          "injspd_1 float NOT NULL DEFAULT '0',"
                                          "injspd_2 float NOT NULL DEFAULT '0',"
                                          "injspd_3 float NOT NULL DEFAULT '0',"
                                          "injspd_4 float NOT NULL DEFAULT '0',"
                                          "injspd_5 float NOT NULL DEFAULT '0',"
                                          "injspd_6 float NOT NULL DEFAULT '0',"
                                          "injspd_7 float NOT NULL DEFAULT '0',"
                                          "injspd_8 float NOT NULL DEFAULT '0',"
                                          "injspd_9 float NOT NULL DEFAULT '0',"
                                          "injspd_10 float NOT NULL DEFAULT '0',"
                                          "injprs_1 float NOT NULL DEFAULT '0',"
                                          "injprs_2 float NOT NULL DEFAULT '0',"
                                          "injprs_3 float NOT NULL DEFAULT '0',"
                                          "injprs_4 float NOT NULL DEFAULT '0',"
                                          "injprs_5 float NOT NULL DEFAULT '0',"
                                          "injprs_6 float NOT NULL DEFAULT '0',"
                                          "injprs_7 float NOT NULL DEFAULT '0',"
                                          "injprs_8 float NOT NULL DEFAULT '0',"
                                          "injprs_9 float NOT NULL DEFAULT '0',"
                                          "injprs_10 float NOT NULL DEFAULT '0',"
                                          "injpos_1 float NOT NULL DEFAULT '0',"
                                          "injpos_2 float NOT NULL DEFAULT '0',"
                                          "injpos_3 float NOT NULL DEFAULT '0',"
                                          "injpos_4 float NOT NULL DEFAULT '0',"
                                          "injpos_5 float NOT NULL DEFAULT '0',"
                                          "injpos_6 float NOT NULL DEFAULT '0',"
                                          "injpos_7 float NOT NULL DEFAULT '0',"
                                          "injpos_8 float NOT NULL DEFAULT '0',"
                                          "injpos_9 float NOT NULL DEFAULT '0',"
                                          "injpos_10 float NOT NULL DEFAULT '0',"
                                          "holdspd_1 float NOT NULL DEFAULT '0',"
                                          "holdspd_2 float NOT NULL DEFAULT '0',"
                                          "holdspd_3 float NOT NULL DEFAULT '0',"
                                          "holdspd_4 float NOT NULL DEFAULT '0',"
                                          "holdspd_5 float NOT NULL DEFAULT '0',"
                                          "holdprs_1 float NOT NULL DEFAULT '0',"
                                          "holdprs_2 float NOT NULL DEFAULT '0',"
                                          "holdprs_3 float NOT NULL DEFAULT '0',"
                                          "holdprs_4 float NOT NULL DEFAULT '0',"
                                          "holdprs_5 float NOT NULL DEFAULT '0',"
                                          "holdtime_1 float NOT NULL DEFAULT '0',"
                                          "holdtime_2 float NOT NULL DEFAULT '0',"
                                          "holdtime_3 float NOT NULL DEFAULT '0',"
                                          "holdtime_4 float NOT NULL DEFAULT '0',"
                                          "holdtime_5 float NOT NULL DEFAULT '0',"
                                          "chgspd_1 float NOT NULL DEFAULT '0',"
                                          "chgspd_2 float NOT NULL DEFAULT '0',"
                                          "chgspd_3 float NOT NULL DEFAULT '0',"
                                          "chgbps_1 float NOT NULL DEFAULT '0',"
                                          "chgbps_2 float NOT NULL DEFAULT '0',"
                                          "chgbps_3 float NOT NULL DEFAULT '0',"
                                          "chgpos_1 float NOT NULL DEFAULT '0',"
                                          "chgpos_2 float NOT NULL DEFAULT '0',"
                                          "chgpos_3 float NOT NULL DEFAULT '0',"
                                          "suckbspd_1 float NOT NULL DEFAULT '0',"
                                          "suckbspd_2 float NOT NULL DEFAULT '0',"
                                          "suckbpos_1 float NOT NULL DEFAULT '0',"
                                          "suckbpos_2 float NOT NULL DEFAULT '0',"
                                          "sovpos float NOT NULL DEFAULT '0',"
                                          "sovprs float NOT NULL DEFAULT '0',"
                                          "injtime float NOT NULL DEFAULT '0',"
                                          "injdeltime float NOT NULL DEFAULT '0',"
                                          "cooltime float NOT NULL DEFAULT '0',"
                                          "chgdeltime float NOT NULL DEFAULT '0',"
                                          "UNIQUE (machine_name)"
                                      ")"
                                      ";");
    }



//    qDebug()<<mysqlquery1.lastQuery();
//    qDebug()<<mysqlquery1.lastError().text();
}

void MainWindow::on_deletebtn_clicked()
{
    QSqlQuery mysqlquery1(mdb);
    if(cellcurrentrow != -1){
        QLabel *machinename = (QLabel *)ui->maintablewidget->cellWidget(cellcurrentrow,ITEMNAME);
        QString machinenametext = machinename->text();
        QString query  = QString("delete from Systeminfo where machine_name = '%1'"
                                 ).arg(machinenametext);
        mysqlquery1.exec(query);
        ui->maintablewidget->removeRow(cellcurrentrow);

        mslotitem *temp_item =  itemmap->value(machinenametext);
        itemmap->remove(machinenametext);
        temp_item->deleteLater();

    }
}

void MainWindow::on_maintablewidget_cellClicked(int row, int column)
{
    cellcurrentrow = row;
    cellcurrentcolumn = column;
}

void MainWindow::on_serverstartbtn_clicked()
{
    ui->ipadderbtn->setEnabled(false);
    ui->deletebtn->setEnabled(false);
    QList<mslotitem *> valuelist = itemmap->values();
    for(int i=0;i<valuelist.size();i++){
        valuelist.at(i)->maintimer.start();
    }

}

void MainWindow::on_serverstopbtn_clicked()
{
    ui->ipadderbtn->setEnabled(true);
    ui->deletebtn->setEnabled(true);
    QList<mslotitem *> valuelist = itemmap->values();
    for(int i=0;i<valuelist.size();i++){
        valuelist.at(i)->maintimer.stop();
        valuelist.at(i)->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        valuelist.at(i)->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
}
void MainWindow::closeEvent(QCloseEvent *event){
    QList<mslotitem *> valuelist = itemmap->values();

    serversetform->deleteLater();

}
