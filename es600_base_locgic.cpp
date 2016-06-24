#include "es600_base_locgic.h"
#include "mslotitem.h"

es600_base_locgic::es600_base_locgic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
}
bool es600_base_locgic::init(){
     mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
     datamap = new QMap<QString,es600value *>;
     ip = parent_item->ip->text();

     remotedb  = QSqlDatabase::database("RemoteDB");

     addrlist.append(mb_object_count);  //0
     addrlist.append(mb_production_count);  //1
     addrlist.append(mb_run_mode);  //2
     addrlist.append(mb_warning_flag); //3
     addrlist.append(mb_cabity);

     addrlist.append(mb_temp1_set);
     temp_set_atnumber = addrlist.count();
     addrlist.append(mb_temp2_set);
     addrlist.append(mb_temp3_set);
     addrlist.append(mb_temp4_set);
     addrlist.append(mb_temp5_set);
     addrlist.append(mb_temp6_set);
     addrlist.append(mb_temp7_set);
     addrlist.append(mb_temp8_set);
     addrlist.append(mb_temp9_set);
     addrlist.append(mb_temp10_set);
     addrlist.append(mb_temp11_set);
     addrlist.append(mb_temp12_set);
     addrlist.append(mb_temp13_set);
     addrlist.append(mb_temp14_set);
     addrlist.append(mb_temp15_set);
     addrlist.append(mb_temp16_set);

     addrlist.append(mb_temp1_up);
     temp_up_atnumber = addrlist.count();
     addrlist.append(mb_temp2_up);
     addrlist.append(mb_temp3_up);
     addrlist.append(mb_temp4_up);
     addrlist.append(mb_temp5_up);
     addrlist.append(mb_temp6_up);
     addrlist.append(mb_temp7_up);
     addrlist.append(mb_temp8_up);
     addrlist.append(mb_temp9_up);
     addrlist.append(mb_temp10_up);
     addrlist.append(mb_temp11_up);
     addrlist.append(mb_temp12_up);
     addrlist.append(mb_temp13_up);
     addrlist.append(mb_temp14_up);
     addrlist.append(mb_temp15_up);
     addrlist.append(mb_temp16_up);

     addrlist.append(mb_temp1_down);
     temp_down_atnumber = addrlist.count();
     addrlist.append(mb_temp2_down);
     addrlist.append(mb_temp3_down);
     addrlist.append(mb_temp4_down);
     addrlist.append(mb_temp5_down);
     addrlist.append(mb_temp6_down);
     addrlist.append(mb_temp7_down);
     addrlist.append(mb_temp8_down);
     addrlist.append(mb_temp9_down);
     addrlist.append(mb_temp10_down);
     addrlist.append(mb_temp11_down);
     addrlist.append(mb_temp12_down);
     addrlist.append(mb_temp13_down);
     addrlist.append(mb_temp14_down);
     addrlist.append(mb_temp15_down);
     addrlist.append(mb_temp16_down);

     addrlist.append(mb_temp1_real);
     temp_real_atnumber = addrlist.count();
     addrlist.append(mb_temp2_real);
     addrlist.append(mb_temp3_real);
     addrlist.append(mb_temp4_real);
     addrlist.append(mb_temp5_real);
     addrlist.append(mb_temp6_real);
     addrlist.append(mb_temp7_real);
     addrlist.append(mb_temp8_real);
     addrlist.append(mb_temp9_real);
     addrlist.append(mb_temp10_real);
     addrlist.append(mb_temp11_real);
     addrlist.append(mb_temp12_real);
     addrlist.append(mb_temp13_real);
     addrlist.append(mb_temp14_real);
     addrlist.append(mb_temp15_real);
     addrlist.append(mb_temp16_real);

     addrlist.append(mb_temp1_onoff);
     temp_onoff_atnumber = addrlist.count();
     addrlist.append(mb_temp2_onoff);
     addrlist.append(mb_temp3_onoff);
     addrlist.append(mb_temp4_onoff);
     addrlist.append(mb_temp5_onoff);
     addrlist.append(mb_temp6_onoff);
     addrlist.append(mb_temp7_onoff);
     addrlist.append(mb_temp8_onoff);
     addrlist.append(mb_temp9_onoff);
     addrlist.append(mb_temp10_onoff);
     addrlist.append(mb_temp11_onoff);
     addrlist.append(mb_temp12_onoff);
     addrlist.append(mb_temp13_onoff);
     addrlist.append(mb_temp14_onoff);
     addrlist.append(mb_temp15_onoff);
     addrlist.append(mb_temp16_onoff);



     modbus_thread = new es600_modbus_thread(this);
     modbus_thread->start();

     initflag=true;
     return initflag;

}
void es600_base_locgic::loop(){
    waitcondition.wakeAll();
}
//es600_modbus_thread 에서 현재 함수를 호출한다.
void es600_base_locgic::es600_base_loop(){
    //example
/*
    qDebug()<<"temp1_set="<<datamap->value(QString("%1").arg(mb_temp1_set))->value;
    qDebug()<<"temp2_set="<<datamap->value(QString("%1").arg(mb_temp2_set))->value;
    qDebug()<<"temp3_set="<<datamap->value(QString("%1").arg(mb_temp3_set))->value;
    qDebug()<<"temp4_set="<<datamap->value(QString("%1").arg(mb_temp4_set))->value;
    qDebug()<<"temp5_set="<<datamap->value(QString("%1").arg(mb_temp5_set))->value;
    qDebug()<<"temp6_set="<<datamap->value(QString("%1").arg(mb_temp6_set))->value;
    qDebug()<<"temp7_set="<<datamap->value(QString("%1").arg(mb_temp7_set))->value;
    qDebug()<<"temp8_set="<<datamap->value(QString("%1").arg(mb_temp8_set))->value;
    qDebug()<<"temp9_set="<<datamap->value(QString("%1").arg(mb_temp9_set))->value;
    qDebug()<<"temp10_set="<<datamap->value(QString("%1").arg(mb_temp10_set))->value;
    qDebug()<<"temp11_set="<<datamap->value(QString("%1").arg(mb_temp11_set))->value;
    qDebug()<<"temp12_set="<<datamap->value(QString("%1").arg(mb_temp12_set))->value;
    qDebug()<<"temp13_set="<<datamap->value(QString("%1").arg(mb_temp13_set))->value;
    qDebug()<<"temp14_set="<<datamap->value(QString("%1").arg(mb_temp14_set))->value;
    qDebug()<<"temp15_set="<<datamap->value(QString("%1").arg(mb_temp15_set))->value;
    qDebug()<<"temp16_set="<<datamap->value(QString("%1").arg(mb_temp16_set))->value;
 */
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QSqlQuery mysqlquery1(remotedb);
    QString update_temp = QString("UPDATE `temp_table` SET ");
    QString temp_append ;
    for(int i=0;i<=15;i++){
    temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6, ")
                            .arg(i)
                            .arg(datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i)))->value)
                            .arg(datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i)))->value)
                            .arg(datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i)))->value)
                            .arg(datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i)))->value)
                            .arg(datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i)))->value);

    }


}
void es600_base_locgic::slot_statue_update(bool statue){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    if(statue){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
    }else {
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
}

