#include "es600_modbus_thread.h"
#include "es600_base_locgic.h"

es600_modbus_thread::es600_modbus_thread(QObject *parent_src){
    playflag = true;
    this->parent = parent_src;
    es600_base_locgic *parent_des = (es600_base_locgic *)parent;

}

void es600_modbus_thread::run(){
    es600_base_locgic *parent_des = (es600_base_locgic *)parent;
    while(playflag){
       parent_des->mutex.lock();
       parent_des->waitcondition.wait(&(parent_des->mutex));
       parent_des->es600_base_loop();
       parent_des->mutex.unlock();
    }
}
