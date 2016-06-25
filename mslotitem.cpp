#include "mslotitem.h"

mslotitem::mslotitem(QString iptext, QString machinenametext, QObject *parent) :
    QObject(parent)
{
    remotedb  = QSqlDatabase::database("remotedb");
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
    status->setTextFormat(Qt::RichText);
    status->setText(tr("<img src=\":/icon/icon/stop.png\">  STOP"));
    QString quertstr2 =  QString("INSERT INTO Systeminfo ("
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

    quertstr2 = QString("select ITEMTYPE from Systeminfo where machine_name = '%1'").arg(machinenametext);

    mysqlquery1.exec(quertstr2);
    if(mysqlquery1.next()){
        type->setCurrentText(mysqlquery1.value("ITEMTYPE").toString());
    }
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

    maintimer.setInterval(MAINTIMERTIME);
    bnr_base_logic = new Bnr_base_locgic(this);
    gefran_base_logic = new gefranseven_base_logic(this);
    Es600_base_locgic = new es600_base_locgic(this);

    connect(type,SIGNAL(currentTextChanged(QString)),this,SLOT(typechange(QString)));
    connect(&maintimer,SIGNAL(timeout()),this,SLOT(maintimer_timeout()));
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
