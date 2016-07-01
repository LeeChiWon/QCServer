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
                         "`cabity` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT ' ',"
                         "`production_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`object_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`achievemen_rate` DOUBLE UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`good_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`poor_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`weight` DOUBLE UNSIGNED NOT NULL DEFAULT '0' COMMENT '',"
                         "`run_mode` TEXT NULL DEFAULT NULL COMMENT '',"
                         "`warning_flag` INT(11) NOT NULL DEFAULT '0',"
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
                         "`temp1_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp1_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp1_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp1_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp1_name` VARCHAR(50) NOT NULL DEFAULT 'temp1' COMMENT ' ' COLLATE 'utf8_bin',"
                         "`temp1_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp2_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp2_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp2_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp2_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp2_name` VARCHAR(50) NOT NULL DEFAULT 'temp2' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp2_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp3_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp3_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp3_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp3_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp3_name` VARCHAR(50) NOT NULL DEFAULT 'temp3' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp3_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp4_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp4_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp4_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp4_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp4_name` VARCHAR(50) NOT NULL DEFAULT 'temp4' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp4_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp5_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp5_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp5_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp5_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp5_name` VARCHAR(50) NOT NULL DEFAULT 'temp5' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp5_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp6_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp6_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp6_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp6_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp6_name` VARCHAR(50) NOT NULL DEFAULT 'temp6' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp6_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp7_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp7_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp7_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp7_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp7_name` VARCHAR(50) NOT NULL DEFAULT 'temp7' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp7_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp8_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp8_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp8_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp8_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp8_name` VARCHAR(50) NOT NULL DEFAULT 'temp8' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp8_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp9_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp9_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp9_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp9_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp9_name` VARCHAR(50) NOT NULL DEFAULT 'temp9' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp9_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp10_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp10_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp10_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp10_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp10_name` VARCHAR(50) NOT NULL DEFAULT 'temp10' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp10_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp11_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp11_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp11_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp11_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp11_name` VARCHAR(50) NOT NULL DEFAULT 'temp11' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp11_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp12_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp12_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp12_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp12_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp12_name` VARCHAR(50) NOT NULL DEFAULT 'temp12' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp12_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp13_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp13_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp13_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp13_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp13_name` VARCHAR(50) NOT NULL DEFAULT 'temp13' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp13_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp14_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp14_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp14_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp14_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp14_name` VARCHAR(50) NOT NULL DEFAULT 'temp14' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp14_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp15_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp15_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp15_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp15_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp15_name` VARCHAR(50) NOT NULL DEFAULT 'temp15' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp15_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp16_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp16_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp16_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp16_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp16_name` VARCHAR(50) NOT NULL DEFAULT 'temp16' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp16_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp17_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp17_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp17_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp17_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp17_name` VARCHAR(50) NOT NULL DEFAULT 'temp17' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp17_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp18_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp18_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp18_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp18_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp18_name` VARCHAR(50) NOT NULL DEFAULT 'temp18' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp18_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp19_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp19_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp19_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp19_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp19_name` VARCHAR(50) NOT NULL DEFAULT 'temp19' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp19_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp20_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp20_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp20_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp20_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp20_name` VARCHAR(50) NOT NULL DEFAULT 'temp20' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp20_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp21_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp21_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp21_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp21_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '',"
                         "`temp21_name` VARCHAR(50) NOT NULL DEFAULT 'temp21' COMMENT '' COLLATE 'utf8_bin',"
                         "`temp21_onoff` INT NOT NULL DEFAULT '0',"
                         "`modift_version` INT(11) NOT NULL DEFAULT '0' COMMENT '',"
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
                                      "`injstep` SMALLINT(6) NOT NULL DEFAULT '0',"
                                      "`holdstep` SMALLINT(6) NOT NULL DEFAULT '0',"
                                      "`injspd_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_6` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_7` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_8` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_9` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injspd_10` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_6` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_7` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_8` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_9` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injprs_10` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_6` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_7` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_8` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_9` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injpos_10` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdspd_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdspd_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdspd_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdspd_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdspd_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdprs_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdprs_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdprs_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdprs_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdprs_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdtime_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdtime_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdtime_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdtime_4` DOUBLE NOT NULL DEFAULT '0',"
                                      "`holdtime_5` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgspd_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgspd_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgspd_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgbps_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgbps_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgbps_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgpos_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgpos_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgpos_3` DOUBLE NOT NULL DEFAULT '0',"
                                      "`suckbspd_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`suckbspd_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`suckbpos_1` DOUBLE NOT NULL DEFAULT '0',"
                                      "`suckbpos_2` DOUBLE NOT NULL DEFAULT '0',"
                                      "`sovpos` DOUBLE NOT NULL DEFAULT '0',"
                                      "`sovprs` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injtime` DOUBLE NOT NULL DEFAULT '0',"
                                      "`injdeltime` DOUBLE NOT NULL DEFAULT '0',"
                                      "`cooltime` DOUBLE NOT NULL DEFAULT '0',"
                                      "`chgdeltime` DOUBLE NOT NULL DEFAULT '0',"
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



    qDebug()<<mysqlquery1.lastQuery();
    qDebug()<<mysqlquery1.lastError().text();
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
#if QT_VERSION > QT_VERSION_CHECK(5,6,0)
    for(int i=0;i<valuelist.count();i++){
        valuelist.at(i)->bnr_base_logic->basepage->deleteLater();
        valuelist.at(i)->bnr_base_logic->optionpage1->deleteLater();
    }
#endif
    serversetform->deleteLater();

}
