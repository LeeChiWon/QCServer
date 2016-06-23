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

     modbus_thread = new es600_modbus_thread(this);
     modbus_thread->start();
     initflag=true;
     return initflag;

}
void es600_base_locgic::loop(){
    waitcondition.wakeAll();
}
//es600_modbus_thread 에서 현재 함수를 호출한다.
//modbus address 추가는 es600_modbus_thread->run()->addrlist.append(주소값)
void es600_base_locgic::es600_base_loop(){
    //example
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

