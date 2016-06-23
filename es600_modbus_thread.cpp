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
       addrlist.append(mb_object_count);
       addrlist.append(mb_production_count);
       addrlist.append(mb_run_mode);
       addrlist.append(mb_warning_flag);
       //addrlist.append(mold_name);
       addrlist.append(mb_cabity);
       addrlist.append(mb_temp1_set);
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

       addrlist.append(mb_temp1_up);
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

       addrlist.append(mb_temp1_real);
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
