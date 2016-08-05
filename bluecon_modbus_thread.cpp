#include "bluecon_modbus_thread.h"
#include "bluecon_base_logic.h"

bluecon_modbus_thread::bluecon_modbus_thread(QObject *parent_src){
    playflag = true;
    this->parent = parent_src;
    bluecon_base_logic *parent_des = (bluecon_base_logic *)parent;
}
void bluecon_modbus_thread::run(){
    bluecon_base_logic *parent_des = (bluecon_base_logic *)parent;
    while(playflag){
       parent_des->mutex.lock();
       parent_des->waitcondition.wait(&(parent_des->mutex));
       parent_des->bluecon_base_loop();
       parent_des->mutex.unlock();
    }
}
