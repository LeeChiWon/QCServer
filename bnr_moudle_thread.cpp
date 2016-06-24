#include "bnr_moudle_thread.h"
#include "bnr_base_locgic.h"

bnr_moudle_thread::bnr_moudle_thread(QObject *parent)
{
    playflag = true;
    this->parent = parent;
}
void bnr_moudle_thread::run(){
    Bnr_base_locgic *parent_logic = (Bnr_base_locgic *)this->parent;
    while(playflag){
        parent_logic->mutex.lock();
        parent_logic->waitcondition.wait(&(parent_logic->mutex));
        parent_logic->url_bnrbaseloop();
        parent_logic->mutex.unlock();
    }
}
