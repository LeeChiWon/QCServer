#include "mslotitem.h"

mslotitem::mslotitem(QString iptext, QString machinenametext, QObject *parent) :
    QObject(parent)
{
    remotedb  = QSqlDatabase::database("remotedb");
    localdb = QSqlDatabase::database("localdb");
    parent_src = (QWidget *)parent;
    QSqlQuery mysqlquery1(remotedb);
    this->iptext = iptext;
    this->machinenametext = machinenametext;
    connectlabel = new QLabel();
    status = new QLabel();
    ip = new QLabel(iptext);
    machinename = new QLabel(machinenametext);
    type = new QComboBox();
    setupbtn = new QPushButton(tr("setup"));
    connectlabel->setTextFormat(Qt::RichText);
    connectlabel->setText(tr("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect"));
    type->addItem("gefranseven/");
    type->addItem("es600/");
    type->addItem("BNR/TAC1XX11");
    type->addItem("BNR/TEC1XX01");
    status->setTextFormat(Qt::RichText);
    status->setText(tr("<img src=\":/icon/icon/stop.png\">  STOP"));
    QSqlQuery litequery1(localdb);
    litequery1.exec("select * from systemset;");
    litequery1.next();
    int typeDB = 0;
    if(litequery1.value("remoteservertype").toString().compare("MYSQL")==0){
        typeDB =MYSQL;
    }else if(litequery1.value("remoteservertype").toString().compare("ODBC")==0){
        typeDB =ODBC;
    }
    QString quertstr2;
    if(typeDB == MYSQL){
    quertstr2 =  QString("INSERT INTO Systeminfo ("
                                 "machine_name,"
                                 "ipaddress,"
                                 "ITEMCONNECT,"
                                 "ITEMSTATUS,"
                                 "ITEMTYPE"
                                 ") values ("
                                 "'%1',"  //machine_name
                                 "'%2',"  // ipaddress
                                 "'%3',"  //ITEMCONNECT
                                 "'%4',"  //ITEMSTATUS
                                 "'%5'"   //ITEMTYPE
                                 ") ON DUPLICATE KEY UPDATE "
                                 "ITEMCONNECT = '%3',"
                                 "ITEMSTATUS = '%4'"
                                 ).arg(machinenametext)
                                  .arg(iptext)
                                  .arg(connectlabel->text())
                                  .arg(status->text())
                                  .arg(type->currentText());
                                  ;
    mysqlquery1.exec(quertstr2);
    }else if(typeDB == ODBC){
   quertstr2 =  QString("UPDATE Systeminfo SET "
                                "ITEMCONNECT = '%3',"
                                "ITEMSTATUS = '%4' "
                                "where machine_name = '%1'"
                                "IF @@ROWCOUNT=0 "
                                "INSERT INTO Systeminfo("
                                "machine_name,"
                                "ipaddress,"
                                "ITEMCONNECT,"
                                "ITEMSTATUS,"
                                "ITEMTYPE"
                                ") values ("
                                "'%1',"  //machine_name
                                "'%2',"  // ipaddress
                                "'%3',"  //ITEMCONNECT
                                "'%4',"  //ITEMSTATUS
                                "'%5')"   //ITEMTYPE
                                ).arg(machinenametext)
                                 .arg(iptext)
                                 .arg(connectlabel->text())
                                 .arg(status->text())
                                 .arg(type->currentText());
                                 ;
        mysqlquery1.exec(quertstr2);
    }

    quertstr2 = QString("select ITEMTYPE from Systeminfo where machine_name = '%1'").arg(machinenametext);

    mysqlquery1.exec(quertstr2);
    if(mysqlquery1.next()){
        type->setCurrentText(mysqlquery1.value("ITEMTYPE").toString());
    }
    if(typeDB == MYSQL){
    quertstr2 = QString("INSERT INTO temp_table (machine_name) values ('%1')"
                        "ON DUPLICATE KEY UPDATE "
                        "`temp1_set`=0 ,`temp1_up`=0 ,`temp1_down`=0 ,`temp1_real`=0 , "
                        "`temp2_set`=0 ,`temp2_up`=0 ,`temp2_down`=0 ,`temp2_real`=0 , "
                        "`temp3_set`=0 ,`temp3_up`=0 ,`temp3_down`=0 ,`temp3_real`=0 , "
                        "`temp4_set`=0 ,`temp4_up`=0 ,`temp4_down`=0 ,`temp4_real`=0 , "
                        "`temp5_set`=0 ,`temp5_up`=0 ,`temp5_down`=0 ,`temp5_real`=0 , "
                        "`temp6_set`=0 ,`temp6_up`=0 ,`temp6_down`=0 ,`temp6_real`=0 , "
                        "`temp7_set`=0 ,`temp7_up`=0 ,`temp7_down`=0 ,`temp7_real`=0 , "
                        "`temp8_set`=0 , `temp8_up`=0 , `temp8_down`=0 , `temp8_real`=0 , "
                        "`temp9_set`=0 , `temp9_up`=0 , `temp9_down`=0 , `temp9_real`=0 , "
                        "`temp10_set`=0 , `temp10_up`=0 , `temp10_down`=0 , `temp10_real`=0 , "
                        "`temp11_set`=0 , `temp11_up`=0 , `temp11_down`=0 , `temp11_real`=0 , "
                        "`temp12_set`=0 , `temp12_up`=0 , `temp12_down`=0 , `temp12_real`=0 , "
                        "`temp13_set`=0 , `temp13_up`=0 , `temp13_down`=0 , `temp13_real`=0 , "
                        "`temp14_set`=0 , `temp14_up`=0 , `temp14_down`=0 , `temp14_real`=0 , "
                        "`temp15_set`=0 , `temp15_up`=0 , `temp15_down`=0 , `temp15_real`=0 , "
                        "`temp16_set`=0 , `temp16_up`=0 , `temp16_down`=0 , `temp16_real`=0 , "
                        "`temp17_set`=0 , `temp17_up`=0 , `temp17_down`=0 , `temp17_real`=0 , "
                        "`temp18_set`=0 , `temp18_up`=0 , `temp18_down`=0 , `temp18_real`=0 , "
                        "`temp19_set`=0 , `temp19_up`=0 , `temp19_down`=0 , `temp19_real`=0 , "
                        "`temp20_set`=0 , `temp20_up`=0 , `temp20_down`=0 , `temp20_real`=0 , "
                        "`temp21_set`=0 , `temp21_up`=0 , `temp21_down`=0 , `temp21_real`=0 "
                        ).arg(machinenametext);
    mysqlquery1.exec(quertstr2);
    }else if(typeDB == ODBC){
        quertstr2 = QString("UPDATE temp_table SET "
                            "temp1_set=NULL ,temp1_up=NULL ,temp1_down=NULL ,temp1_real=NULL , "
                            "temp2_set=NULL ,temp2_up=NULL ,temp2_down=NULL ,temp2_real=NULL , "
                            "temp3_set=NULL ,temp3_up=NULL ,temp3_down=NULL ,temp3_real=NULL , "
                            "temp4_set=NULL ,temp4_up=NULL ,temp4_down=NULL ,temp4_real=NULL , "
                            "temp5_set=NULL ,temp5_up=NULL ,temp5_down=NULL ,temp5_real=NULL , "
                            "temp6_set=NULL ,temp6_up=NULL ,temp6_down=NULL ,temp6_real=NULL , "
                            "temp7_set=NULL ,temp7_up=NULL ,temp7_down=NULL ,temp7_real=NULL , "
                            "temp8_set=NULL , temp8_up=NULL , temp8_down=NULL , temp8_real=NULL , "
                            "temp9_set=NULL , temp9_up=NULL , temp9_down=NULL , temp9_real=NULL , "
                            "temp10_set=NULL , temp10_up=NULL , temp10_down=NULL , temp10_real=NULL , "
                            "temp11_set=NULL , temp11_up=NULL , temp11_down=NULL , temp11_real=NULL , "
                            "temp12_set=NULL , temp12_up=NULL , temp12_down=NULL , temp12_real=NULL , "
                            "temp13_set=NULL , temp13_up=NULL , temp13_down=NULL , temp13_real=NULL , "
                            "temp14_set=NULL , temp14_up=NULL , temp14_down=NULL , temp14_real=NULL , "
                            "temp15_set=NULL , temp15_up=NULL , temp15_down=NULL , temp15_real=NULL , "
                            "temp16_set=NULL , temp16_up=NULL , temp16_down=NULL , temp16_real=NULL , "
                            "temp17_set=NULL , temp17_up=NULL , temp17_down=NULL , temp17_real=NULL , "
                            "temp18_set=NULL , temp18_up=NULL , temp18_down=NULL , temp18_real=NULL , "
                            "temp19_set=NULL , temp19_up=NULL , temp19_down=NULL , temp19_real=NULL , "
                            "temp20_set=NULL , temp20_up=NULL , temp20_down=NULL , temp20_real=NULL , "
                            "temp21_set=NULL , temp21_up=NULL , temp21_down=NULL , temp21_real=NULL "
                            "where machine_name = '%1' "
                            "IF @@ROWCOUNT=0 "
                            "INSERT INTO temp_table(machine_name) values('%1')"
                            ).arg(machinenametext);
         mysqlquery1.exec(quertstr2);

    }
    if(typeDB == MYSQL){
    quertstr2=QString("insert into Recipe_Info (machine_name) values ('%1')"
                      "on duplicate key update "
                      "`injstep`=0, `holdstep`=0,"
                      "`injspd_1`=0, `injspd_2`=0, `injspd_3`=0, `injspd_4`=0, `injspd_5`=0,"
                      "`injspd_6`=0, `injspd_7`=0, `injspd_8`=0, `injspd_9`=0, `injspd_10`=0,"
                      "`injprs_1`=0, `injprs_2`=0, `injprs_3`=0, `injprs_4`=0, `injprs_5`=0,"
                      "`injprs_6`=0, `injprs_7`=0, `injprs_8`=0, `injprs_9`=0, `injprs_10`=0,"
                      "`injpos_1`=0, `injpos_2`=0, `injpos_3`=0, `injpos_4`=0, `injpos_5`=0,"
                      "`injpos_6`=0, `injpos_7`=0, `injpos_8`=0, `injpos_9`=0, `injpos_10`=0,"
                      "`holdspd_1`=0, `holdspd_2`=0, `holdspd_3`=0, `holdspd_4`=0, `holdspd_5`=0,"
                      "`holdprs_1`=0, `holdprs_2`=0, `holdprs_3`=0, `holdprs_4`=0, `holdprs_5`=0,"
                      "`holdtime_1`=0, `holdtime_2`=0, `holdtime_3`=0, `holdtime_4`=0, `holdtime_5`=0,"
                      "`chgspd_1`=0, `chgspd_2`=0, `chgspd_3`=0,"
                      "`chgbps_1`=0, `chgbps_2`=0, `chgbps_3`=0,"
                      "`chgpos_1`=0, `chgpos_2`=0, `chgpos_3`=0,"
                      "`suckbspd_1`=0, `suckbspd_2`=0, `suckbpos_1`=0, `suckbpos_2`=0,"
                      "`sovpos`=0, `sovprs`=0, `injtime`=0, `injdeltime`=0, `cooltime`=0, `chgdeltime`=0").arg(machinenametext);
    mysqlquery1.exec(quertstr2);
    }else if(typeDB == ODBC){
    quertstr2=QString("UPDATE Recipe_Info SET "
                      "injspd_1=NULL, injspd_2=NULL, injspd_3=NULL, injspd_4=NULL, injspd_5=NULL,"
                      "injspd_6=NULL, injspd_7=NULL, injspd_8=NULL, injspd_9=NULL, injspd_10=NULL,"
                      "injprs_1=NULL, injprs_2=NULL, injprs_3=NULL, injprs_4=NULL, injprs_5=NULL,"
                      "injprs_6=NULL, injprs_7=NULL, injprs_8=NULL, injprs_9=NULL, injprs_10=NULL,"
                      "injpos_1=NULL, injpos_2=NULL, injpos_3=NULL, injpos_4=NULL, injpos_5=NULL,"
                      "injpos_6=NULL, injpos_7=NULL, injpos_8=NULL, injpos_9=NULL, injpos_10=NULL,"
                      "holdspd_1=NULL, holdspd_2=NULL, holdspd_3=NULL, holdspd_4=NULL, holdspd_5=NULL,"
                      "holdprs_1=NULL, holdprs_2=NULL, holdprs_3=NULL, holdprs_4=NULL, holdprs_5=NULL,"
                      "holdtime_1=NULL, holdtime_2=NULL, holdtime_3=NULL, holdtime_4=NULL, holdtime_5=NULL,"
                      "chgspd_1=NULL, chgspd_2=NULL, chgspd_3=NULL,"
                      "chgbps_1=NULL, chgbps_2=NULL, chgbps_3=NULL,"
                      "chgpos_1=NULL, chgpos_2=NULL, chgpos_3=NULL,"
                      "suckbspd_1=NULL, suckbspd_2=NULL, suckbpos_1=NULL, suckbpos_2=NULL,"
                      "sovpos=NULL, sovprs=NULL, injtime=NULL, injdeltime=NULL, cooltime=NULL, chgdeltime=NULL "
                      "where machine_name = '%1' "
                      "IF @@ROWCOUNT=0 "
                      "INSERT INTO Recipe_Info(machine_name) values('%1')"
                      ).arg(machinenametext);
    mysqlquery1.exec(quertstr2);
    }
    maintimer.setInterval(MAINTIMERTIME);
    bnr_base_logic = new Bnr_base_locgic(this);
    gefran_base_logic = new gefranseven_base_logic(this);
    Es600_base_locgic = new es600_base_locgic(this);

    gefranset_popup = 0;

    connect(type,SIGNAL(currentTextChanged(QString)),this,SLOT(typechange(QString)));
    connect(&maintimer,SIGNAL(timeout()),this,SLOT(maintimer_timeout()));
    connect(setupbtn,SIGNAL(clicked(bool)),this,SLOT(setupbtn_click(bool)));
}
void mslotitem::typechange(QString data){
    QSqlQuery mysqlquery1(remotedb);

    QString quertstr2 = QString("UPDATE Systeminfo set ITEMTYPE = '%1' "
                                "where machine_name='%2'")
                                .arg(data)
                                .arg(machinenametext);

    mysqlquery1.exec(quertstr2);
}
//maintimer loop
void mslotitem::maintimer_timeout(){

    if(type->currentText().split("/").at(0).compare("BNR")==0){
        if(!bnr_base_logic->initflag){
            bnr_base_logic->init();
        }
        //loop logic
        bnr_base_logic->loop();
    }else if(type->currentText().split("/").at(0).compare("gefranseven")==0){
        if(!gefran_base_logic->initflag){
            gefran_base_logic->init();
        }
        gefran_base_logic->loop();

    }else if(type->currentText().split("/").at(0).compare("es600")==0){
        if(!Es600_base_locgic->initflag){
            Es600_base_locgic->init();
        }
        Es600_base_locgic->loop();
    }
}

void mslotitem::set_connectlabel_text(QString data){
    connectlabel->setText(data);
    QSqlQuery mysqlquery1(remotedb);
    QString quertstr2 = QString("UPDATE Systeminfo set ITEMCONNECT = '%1' "
                                "where machine_name='%2'")
                                .arg(data)
                                .arg(machinenametext);
    mysqlquery1.exec(quertstr2);

}
void mslotitem::set_status_text(QString data){
    status->setText(data);
    QSqlQuery mysqlquery1(remotedb);
    QString quertstr2 = QString("UPDATE Systeminfo set ITEMSTATUS = '%1' "
                                "where machine_name='%2'")
                                .arg(data)
                                .arg(machinenametext);
    mysqlquery1.exec(quertstr2);
}
void mslotitem::setupbtn_click(bool result){
    if(type->currentText().split("/").at(0).compare("gefranseven")==0){
        if(gefranset_popup == 0){
            gefranset_popup = new gefransetup_popup(machinenametext);
            gefranset_popup->show();
        }else{
            gefranset_popup->show();
        }
    }
}
