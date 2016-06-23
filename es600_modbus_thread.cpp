#include "es600_modbus_thread.h"
#include "es600_base_locgic.h"



es600_modbus_thread::es600_modbus_thread(QObject *parent_src){
    playflag = true;
    this->parent = parent_src;
    es600_base_locgic *parent_des = (es600_base_locgic *)parent;

    parent_des->ctx = modbus_new_tcp(parent_des->ip.toLocal8Bit(),502);
    modbus_set_slave(parent_des->ctx,255);
    connect(this,SIGNAL(sig_statue_update(bool)),parent_des,SLOT(slot_statue_update(bool)));

}

void es600_modbus_thread::run(){
    es600_base_locgic *parent_des = (es600_base_locgic *)parent;
    QVector<int> addrlist;
    while(playflag){
       parent_des->mutex.lock();
       parent_des->waitcondition.wait(&(parent_des->mutex));
       addrlist.clear();
       //주소 추가
       addrlist.append(114);
       addrlist.append(116);
       addrlist.append(118);
       addrlist.append(120);
       addrlist.append(122);
       addrlist.append(124);
       addrlist.append(126);
       addrlist.append(128);
       addrlist.append(130);
       addrlist.append(132);
       addrlist.append(134);
       addrlist.append(136);
       addrlist.append(138);

       for(int i=0;i<addrlist.count();i++){
           if(modbus_read_registers(parent_des->ctx,addrlist.at(i),2,reg) == -1){
               if(modbus_connect(parent_des->ctx) == -1){
                   qDebug()<<parent_des->ip<<"="<<"modbus connect fail";
                    emit sig_statue_update(false);
               }else {
                    emit sig_statue_update(true);
               }
               QString addr = QString("%1").arg(addrlist.at(i));
               if(!parent_des->datamap->contains(addr)){
                     parent_des->datamap->insert(addr,new es600value(addr,QString("%1").arg(0)));
               }else{
                      es600value *tempvalue = parent_des->datamap->value(addr);
                      tempvalue->value = QString("%1").arg(0);
               }
             }else {
                 QString addr = QString("%1").arg(addrlist.at(i));
                 if(!parent_des->datamap->contains(addr)){
                       parent_des->datamap->insert(addr,new es600value(addr,QString("%1").arg(reg[0])));
                 }else{
                        es600value *tempvalue = parent_des->datamap->value(addr);
                        tempvalue->value = QString("%1").arg(reg[0]);
                 }
             }
       }

       parent_des->es600_base_loop();
       parent_des->mutex.unlock();
    }
}
