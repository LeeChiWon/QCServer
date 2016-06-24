#include "gefranseven_moudle_thread.h"
#include "gefranseven_base_logic.h"
gefranseven_moudle_thread::gefranseven_moudle_thread(QObject *parent)
{
    playflag = true;
    this->parent = parent;
}
void gefranseven_moudle_thread::run(){
    gefranseven_base_logic *parent_logic = (gefranseven_base_logic *)this->parent;
    while(playflag){
        parent_logic->mutex.lock();
        parent_logic->waitcondition.wait(&(parent_logic->mutex));
        parent_logic->url_gefranbaseloop();
        parent_logic->mutex.unlock();
    }

}
