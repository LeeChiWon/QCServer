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
                    "remoteserveruserpassword TEXT"
                    ");");
    //만약조건이없다면 업데이트
    litequery1.exec("insert into systemset(remoteserverip,"
                    "remoteserverport,"
                    "remoteserverdbname,"
                    "version,"
                    "remoteserverusername,"
                    "remoteserveruserpassword) "
                    "select \'127.0.0.1\',"
                    "\'3306\',"
                    "\'QCproject\',"
                    "1,"
                    "\'QCmen\',"
                    "\'1234\' "
                    "where not exists(select * from systemset);");

}
void MainWindow::remotesql_connect(){
    QSqlQuery litequery1(litedb);
    litequery1.exec("select * from systemset;");
    litequery1.next();
    mdb =QSqlDatabase::addDatabase("QMYSQL","remotedb");
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
}

void MainWindow::remotesql_init(){
    QSqlQuery mysqlquery1(mdb);
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
    mysqlquery1.exec("CREATE TABLE `Systeminfo` ("
                         "`machine_name` VARCHAR(50) NULL DEFAULT NULL COMMENT '기계이름',"
                         "`ipaddress` TEXT NULL DEFAULT NULL COMMENT 'ip주소',"
                         "`ITEMCONNECT` TEXT NULL DEFAULT NULL COMMENT '연결상태',"
                         "`ITEMSTATUS` TEXT NULL DEFAULT NULL COMMENT '작동상황',"
                         "`ITEMTYPE` TEXT NULL DEFAULT NULL COMMENT '모니터타입',"
                         "`mold_name` TEXT NULL DEFAULT NULL COMMENT '금형이름',"
                         "`worker` TEXT NULL DEFAULT NULL COMMENT '작업자',"
                         "`item_code` TEXT NULL DEFAULT NULL COMMENT '품목코드',"
                         "`item_name` TEXT NULL DEFAULT NULL COMMENT '품목명',"
                         "`orders_count` TEXT NULL DEFAULT NULL COMMENT '지시수량',"
                         "`cycle_time` TIME NULL DEFAULT NULL COMMENT '사이클 타임',"
                         "`cabity` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '캐비티 수량',"
                         "`production_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '생산량',"
                         "`object_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '목표수량',"
                         "`achievemen_rate` DOUBLE UNSIGNED NOT NULL DEFAULT '0' COMMENT '달성률',"
                         "`good_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '양품',"
                         "`poor_count` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '불량',"
                         "`weight` DOUBLE UNSIGNED NOT NULL DEFAULT '0' COMMENT '중량',"
                         "`run_mode` TEXT NULL DEFAULT NULL COMMENT '작동모드',"
                         "`warning_flag` INT(11) NOT NULL DEFAULT '0',"
                         "UNIQUE INDEX `machine_name` (`machine_name`)"
                     ")"
                      "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                   );
    mysqlquery1.exec("CREATE TABLE `mold_info` ("
                         "`mold_name` VARCHAR(50) NULL DEFAULT NULL COMMENT '금형이름',"
                         "`item_name` TEXT NULL DEFAULT NULL COMMENT '제품이름',"
                         "`item_code` TEXT NULL DEFAULT NULL COMMENT '제품코드',"
                         "UNIQUE INDEX `mold_name` (`mold_name`)"
                     ")"
                    "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                );
    mysqlquery1.exec(tr("CREATE TABLE `temp_table` ("
                         "`machine_name` VARCHAR(50) NULL DEFAULT NULL,"
                         "`temp1_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp1_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp1_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp1_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp1_name` VARCHAR(50) NOT NULL DEFAULT '온도1' COMMENT '온도이름 ' COLLATE 'utf8_bin',"
                         "`temp1_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp2_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp2_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp2_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp2_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp2_name` VARCHAR(50) NOT NULL DEFAULT '온도2' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp2_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp3_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp3_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp3_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp3_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp3_name` VARCHAR(50) NOT NULL DEFAULT '온도3' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp3_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp4_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp4_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp4_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp4_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp4_name` VARCHAR(50) NOT NULL DEFAULT '온도4' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp4_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp5_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp5_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp5_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp5_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp5_name` VARCHAR(50) NOT NULL DEFAULT '온도5' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp5_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp6_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp6_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp6_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp6_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp6_name` VARCHAR(50) NOT NULL DEFAULT '온도6' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp6_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp7_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp7_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp7_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp7_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp7_name` VARCHAR(50) NOT NULL DEFAULT '온도7' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp7_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp8_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp8_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp8_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp8_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp8_name` VARCHAR(50) NOT NULL DEFAULT '온도8' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp8_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp9_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp9_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp9_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp9_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp9_name` VARCHAR(50) NOT NULL DEFAULT '온도9' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp9_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp10_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp10_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp10_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp10_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp10_name` VARCHAR(50) NOT NULL DEFAULT '온도10' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp10_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp11_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp11_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp11_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp11_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp11_name` VARCHAR(50) NOT NULL DEFAULT '온도11' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp11_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp12_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp12_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp12_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp12_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp12_name` VARCHAR(50) NOT NULL DEFAULT '온도12' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp12_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp13_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp13_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp13_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp13_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp13_name` VARCHAR(50) NOT NULL DEFAULT '온도13' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp13_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp14_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp14_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp14_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp14_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp14_name` VARCHAR(50) NOT NULL DEFAULT '온도14' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp14_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp15_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp15_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp15_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp15_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp15_name` VARCHAR(50) NOT NULL DEFAULT '온도15' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp15_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp16_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp16_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp16_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp16_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp16_name` VARCHAR(50) NOT NULL DEFAULT '온도16' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp16_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp17_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp17_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp17_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp17_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp17_name` VARCHAR(50) NOT NULL DEFAULT '온도17' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp17_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp18_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp18_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp18_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp18_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp18_name` VARCHAR(50) NOT NULL DEFAULT '온도18' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp18_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp19_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp19_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp19_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp19_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp19_name` VARCHAR(50) NOT NULL DEFAULT '온도19' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp19_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp20_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp20_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp20_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp20_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp20_name` VARCHAR(50) NOT NULL DEFAULT '온도20' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp20_onoff` INT NOT NULL DEFAULT '0',"
                         "`temp21_set` DOUBLE NOT NULL DEFAULT '0' COMMENT '설정온도',"
                         "`temp21_up` DOUBLE NOT NULL DEFAULT '0' COMMENT '상한온도',"
                         "`temp21_down` DOUBLE NOT NULL DEFAULT '0' COMMENT '하한온도',"
                         "`temp21_real` DOUBLE NOT NULL DEFAULT '0' COMMENT '실제값',"
                         "`temp21_name` VARCHAR(50) NOT NULL DEFAULT '온도21' COMMENT '온도이름' COLLATE 'utf8_bin',"
                         "`temp21_onoff` INT NOT NULL DEFAULT '0',"
                         "`modift_version` INT(11) NOT NULL DEFAULT '0' COMMENT '버전',"
                         "UNIQUE INDEX `machine_name` (`machine_name`)"
                     ")"
                    "COLLATE='utf8_bin'"
                     "ENGINE=InnoDB"
                     ";"
                ));


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
    for(int i=0;i<valuelist.count();i++){
        valuelist.at(i)->bnr_base_logic->basepage->deleteLater();
        valuelist.at(i)->bnr_base_logic->optionpage1->deleteLater();
    }
    serversetform->deleteLater();

}
