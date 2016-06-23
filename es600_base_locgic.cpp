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
    //datamap->value("114")->value

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

