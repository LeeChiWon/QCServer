#include "bluecon_base_logic.h"
#include "mslotitem.h"

bluecon_base_logic::bluecon_base_logic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
    modbuscount=0;
    before_shotcount =-1;
    current_shotcount =-1;
}

bool bluecon_base_logic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,blueconvalue *>;
    alrammap = new QMap<QString,alrammap_data *> ;
    litedb = QSqlDatabase::database("localdb");
    QSqlQuery litequery1(litedb);
    litequery1.exec("select * from systemset;");
    litequery1.next();
    if(litequery1.value("remoteservertype").toString().compare("MYSQL")==0){
        remotedb = QSqlDatabase::addDatabase("QMYSQL",parent_item->iptext);
    }else if("ODBC"){
        remotedb = QSqlDatabase::addDatabase("QODBC",parent_item->iptext);
    }
    remotedb.setHostName(litequery1.value("remoteserverip").toString());
    remotedb.setDatabaseName(litequery1.value("remoteserverdbname").toString());
    remotedb.setPort(litequery1.value("remoteserverport").toInt());
    remotedb.setUserName(litequery1.value("remoteserverusername").toString());
    remotedb.setPassword(litequery1.value("remoteserveruserpassword").toString());
    if(!remotedb.open()){
        qDebug()<<"bluecon DB not open";

    }else {
       qDebug()<<"bluecon DB open";
    }
    QSqlQuery remotequery(remotedb);
    remotequery.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(parent_item->machinenametext));
    if(remotequery.next()){
        QString optionvalue = remotequery.value("option_data").toString();
    }
    qctx = new QModbusTcpClient(this);
    qctx->setConnectionParameter(QModbusDevice::NetworkAddressParameter,parent_item->iptext);
    qctx->setConnectionParameter(QModbusDevice::NetworkPortParameter,502);
    qctx->setTimeout(3000);
    if(!qctx->connectDevice()){
        qDebug()<<"gefran qctx connect false";
    }else {

    }

    connect(qctx,&QModbusClient::stateChanged,this,&bluecon_base_logic::modbusstatue_change);
    addrlist.append(bmb_object_count);
    addrlist.append(bmb_production_count);
    addrlist.append(bmb_run_mode);
    addrlist.append(bmb_warning_flag);
    addrlist.append(bmb_cabity);
    addrlist.append(bmb_moldname1);
    addrlist.append(bmb_moldname2);
    addrlist.append(bmb_moldname3);
    addrlist.append(bmb_moldname4);
    addrlist.append(bmb_moldname5);
    addrlist.append(bmb_moldname6);
    addrlist.append(bmb_moldname7);
    addrlist.append(bmb_moldname8);
    addrlist.append(bmb_moldname9);
    addrlist.append(bmb_moldname10);

    addrlist.append(bmb_injVelocity1);
    addrlist.append(bmb_injVelocity2);
    addrlist.append(bmb_injVelocity3);
    addrlist.append(bmb_injVelocity4);
    addrlist.append(bmb_injVelocity5);
    addrlist.append(bmb_injVelocity6);
    addrlist.append(bmb_injVelocity7);
    addrlist.append(bmb_injVelocity8);
    addrlist.append(bmb_injVelocity9);
    addrlist.append(bmb_injVelocity10);

    addrlist.append(bmb_injPressure1);
    addrlist.append(bmb_injPressure2);
    addrlist.append(bmb_injPressure3);
    addrlist.append(bmb_injPressure4);
    addrlist.append(bmb_injPressure5);
    addrlist.append(bmb_injPressure6);
    addrlist.append(bmb_injPressure7);
    addrlist.append(bmb_injPressure8);
    addrlist.append(bmb_injPressure9);
    addrlist.append(bmb_injPressure10);

    addrlist.append(bmb_injPosition1);
    addrlist.append(bmb_injPosition2);
    addrlist.append(bmb_injPosition3);
    addrlist.append(bmb_injPosition4);
    addrlist.append(bmb_injPosition5);
    addrlist.append(bmb_injPosition6);
    addrlist.append(bmb_injPosition7);
    addrlist.append(bmb_injPosition8);
    addrlist.append(bmb_injPosition9);
    addrlist.append(bmb_injPosition10);

    addrlist.append(bmb_sov_time);
    addrlist.append(bmb_sov_position);

    addrlist.append(bmb_hldPressure1);
    addrlist.append(bmb_hldPressure2);
    addrlist.append(bmb_hldPressure3);
    addrlist.append(bmb_hldPressure4);
    addrlist.append(bmb_hldPressure5);

    addrlist.append(bmb_hldTime1);
    addrlist.append(bmb_hldTime2);
    addrlist.append(bmb_hldTime3);
    addrlist.append(bmb_hldTime4);
    addrlist.append(bmb_hldTime5);

    addrlist.append(bmb_hldVel1);
    addrlist.append(bmb_hldVel2);
    addrlist.append(bmb_hldVel3);
    addrlist.append(bmb_hldVel4);
    addrlist.append(bmb_hldVel5);

    addrlist.append(bmb_chgPosition1);
    addrlist.append(bmb_chgPosition2);
    addrlist.append(bmb_chgPosition3);
    addrlist.append(bmb_chgPosition4);

    addrlist.append(bmb_chgSpeed1);
    addrlist.append(bmb_chgSpeed2);
    addrlist.append(bmb_chgSpeed3);
    addrlist.append(bmb_chgSpeed4);

    addrlist.append(bmb_backPressure1);
    addrlist.append(bmb_backPressure2);
    addrlist.append(bmb_backPressure3);
    addrlist.append(bmb_backPressure4);

    addrlist.append(bmb_suckbackPosition1);
    addrlist.append(bmb_suckbackPosition2);

    addrlist.append(bmb_suckbackSpeed1);
    addrlist.append(bmb_suckbackSpeed2);

    addrlist.append(bmb_temp1_set);
    addrlist.append(bmb_temp2_set);
    addrlist.append(bmb_temp3_set);
    addrlist.append(bmb_temp4_set);
    addrlist.append(bmb_temp5_set);
    addrlist.append(bmb_temp6_set);
    addrlist.append(bmb_temp7_set);
    addrlist.append(bmb_temp8_set);
    addrlist.append(bmb_temp9_set);
    addrlist.append(bmb_temp10_set);
    addrlist.append(bmb_temp11_set);
    addrlist.append(bmb_temp12_set);
    addrlist.append(bmb_temp13_set);
    addrlist.append(bmb_temp14_set);
    addrlist.append(bmb_temp15_set);
    addrlist.append(bmb_temp16_set);

    addrlist.append(bmb_INJETM);
    addrlist.append(bmb_COOLTM);
    addrlist.append(bmb_SPR2TM);
    addrlist.append(bmb_CHDLTM);

    addrlist.append(bmb_injstep);
    addrlist.append(bmb_hldstep);

    addrlist.append(bmb_SHOTDATA_fooldata);
    addrlist.append(bmb_SHOTDATA_inctiontime);
    addrlist.append(bmb_SHOTDATA_fillingtime);
    addrlist.append(bmb_SHOTDATA_plasticizing_time);
    addrlist.append(bmb_SHOTDATA_cycle_time);
    addrlist.append(bmb_SHOTDATA_clamp_close_time);
    addrlist.append(bmb_SHOTDATA_cushion_position);
    addrlist.append(bmb_SHOTDATA_switch_over_position);
    addrlist.append(bmb_SHOTDATA_plasticizing_position);
    addrlist.append(bmb_SHOTDATA_clamp_open_position);
    addrlist.append(bmb_SHOTDATA__max_injection_speed);
    addrlist.append(bmb_SHOTDATA__max_screw_RPM);
    addrlist.append(bmb_SHOTDATA__average_screw_RPM);
    addrlist.append(bmb_SHOTDATA__max_injection_pressure);
    addrlist.append(bmb_SHOTDATA__max_switch_over_pressure);
    addrlist.append(bmb_SHOTDATA__max_back_pressure);
    addrlist.append(bmb_SHOTDATA__average_back_pressure);

    addrlist.append(bmb_alrammap1);
    addrlist.append(bmb_alrammap2);
    addrlist.append(bmb_alrammap3);
    addrlist.append(bmb_alrammap4);
    addrlist.append(bmb_alrammap5);
    addrlist.append(bmb_alrammap6);
    addrlist.append(bmb_alrammap7);
    addrlist.append(bmb_alrammap8);
    addrlist.append(bmb_alrammap9);
    addrlist.append(bmb_alrammap10);

    addrlist.append(bmb_temp1_real);
    addrlist.append(bmb_temp2_real);
    addrlist.append(bmb_temp3_real);
    addrlist.append(bmb_temp4_real);
    addrlist.append(bmb_temp5_real);
    addrlist.append(bmb_temp6_real);
    addrlist.append(bmb_temp7_real);
    addrlist.append(bmb_temp8_real);
    addrlist.append(bmb_temp9_real);
    addrlist.append(bmb_temp10_real);
    addrlist.append(bmb_temp11_real);
    addrlist.append(bmb_temp12_real);
    addrlist.append(bmb_temp13_real);
    addrlist.append(bmb_temp14_real);
    addrlist.append(bmb_temp15_real);
    addrlist.append(bmb_temp16_real);

    for(int i=0;i<addrlist.size();i++){
        datamap->insert(QString("%1").arg(addrlist.at(i))
                        ,new blueconvalue(QString("%1").arg(addrlist.at(i)),QString("%1").arg(-1)));
    }

    modbus_thread = new bluecon_modbus_thread(this);
    modbus_thread->start();

    initflag=true;
    return initflag;
}

void bluecon_base_logic::loop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    if(modbuscount == 0){
        if(qctx->state()==QModbusDevice::ConnectedState){
            slot_statue_update(true);
            QModbusReply *reply;
            reply = qctx->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,addrlist.at(modbuscount),2),255);
            connect(reply,SIGNAL(finished()),this,SLOT(modbusread_ready()));
            modbuscount++;
        }
    }
}
void bluecon_base_logic::bluecon_base_loop(){
    //qDebug()<<"test";
    alram_update();
    REC_save();
}

void bluecon_base_logic::modbusread_ready(){

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
            reply->deleteLater();
            return;
    }
    const QModbusDataUnit unit = reply->result();
    if (reply->error() == QModbusDevice::NoError) {
        int startaddress = unit.startAddress();
        int value = unit.value(0);
        QString startaddress_str = QString("%1").arg(startaddress);
        if(!datamap->contains(startaddress_str)){
            datamap->insert(startaddress_str,new blueconvalue(startaddress_str,QString("%1").arg(value)));
        }else {
            blueconvalue *tempvalue = datamap->value(startaddress_str);
            tempvalue->value = QString("%1").arg(value);
        }
        if(startaddress == addrlist.last()){  //마지막 데이터 가지 받으면 loop 실행
            waitcondition.wakeAll();
        }
    }else {
//        qDebug()<<"modbus error = "<<reply->errorString();

    }

    if(modbuscount >= addrlist.size()){
        modbuscount = 0;
    }else {
        if(qctx->state()==QModbusDevice::ConnectedState){
            QModbusReply *reply;
            reply = qctx->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,addrlist.at(modbuscount),2),255);
            connect(reply,SIGNAL(finished()),this,SLOT(modbusread_ready()));
            modbuscount++;
        }
    }
    reply->deleteLater();
}

void bluecon_base_logic::current_update(){

}

void bluecon_base_logic::REC_save(){
    if(before_shotcount<0){
        before_shotcount = current_shotcount;
        //modbus_write_register(ctx,mb_actstatus,0);
    }
    if(before_shotcount!=current_shotcount){
        before_shotcount=current_shotcount;
         QString mold_name = get_mold_name();
         int fooldata =datamap->value(QString("%1").arg(bmb_SHOTDATA_fooldata))->value.toInt();
         double injectiontime = datamap->value(QString("%1").arg(bmb_SHOTDATA_inctiontime))->value.toDouble()/10.0;
         double fillingtime = datamap->value(QString("%1").arg(mb_SHOTDATA_fillingtime))->value.toDouble()/100.0;
    }


}
void bluecon_base_logic::alram_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString monitertype = parent_item->type->currentText();
    int alramflagdata = datamap->value(QString("%1").arg(bmb_warning_flag))->value.toInt();
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery mysqlquery1(remotedb);
    QStringList maplist = alrammap->keys();
    for(int j=0;j<maplist.size();j++){
        alrammap_data *tempmapdata;
        tempmapdata = alrammap->value(maplist.at(j));
        tempmapdata->currentflag = false;
    }
    if(alramflagdata > 0){
        for(int i=0;i<alramflagdata;i++){
            QString temp_alramdata = datamap->value(QString("%1").arg(bmb_alrammap1+(i*2)))->value;
            alrammap_data *tempmapdata;
            if(!alrammap->contains(temp_alramdata)){
                tempmapdata = new alrammap_data();
                alrammap->insert(temp_alramdata,tempmapdata);
            }else{
                tempmapdata = alrammap->value(temp_alramdata);
            }
            tempmapdata->currentflag = true;
        }
    }else {
        maplist = alrammap->keys();
        for(int k=0;k<maplist.size();k++){
            alrammap_data *tempmapdata;
            tempmapdata = alrammap->value(maplist.at(k));
            tempmapdata->currentflag = false;
        }
    }
    maplist = alrammap->keys();
    for(int l=0;l<maplist.size();l++){
        alrammap_data *tempmapdata;
        tempmapdata = alrammap->value(maplist.at(l));
        if(tempmapdata->beforeflag != tempmapdata->currentflag){
            tempmapdata->beforeflag = tempmapdata->currentflag;
            if(tempmapdata->currentflag){
                mysqlquery1.exec("INSERT INTO "
                                 "Alarm_Log (Machine_Name,"
                                 "Controller_Info,"
                                 "Alarm_Number,"
                                 "Alarm_Start_Time,"
                                 "Alarm_End_Time,"
                                 "Alarm_flag) "
                                 "VALUES "
                                 "('"+mancine_name+"',"
                                 "'"+monitertype+"', "
                                 ""+maplist.at(l)+", "
                                 "'"+datetime+"', "
                                 "'1999-01-01 00:00:00',"
                                 "1)"
                                 ";"
                            );
            }else {
                mysqlquery1.exec("INSERT INTO "
                                 "Alarm_Log (Machine_Name,"
                                 "Controller_Info,"
                                 "Alarm_Number,"
                                 "Alarm_Start_Time,"
                                 "Alarm_End_Time,"
                                 "Alarm_flag) "
                                 "VALUES "
                                 "('"+mancine_name+"',"
                                 "'"+monitertype+"', "
                                 ""+maplist.at(l)+", "
                                 "'1999-01-01 00:00:00', "
                                 "'"+datetime+"',"
                                 "0)"
                                 ";"
                           );
            }
        }
    }


}

void bluecon_base_logic::modbusstatue_change(int state){

    if(state == QModbusDevice::ConnectedState){
        slot_statue_update(true);
    }else if(state == QModbusDevice::ConnectingState){
        slot_statue_update(false);
    }else if(state == QModbusDevice::UnconnectedState){
        qctx->connectDevice();
        slot_statue_update(false);
    }
    modbuscount = 0;
}

void bluecon_base_logic::slot_statue_update(bool statue){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    if(statue){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
    }else {
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
}
QString bluecon_base_logic::get_mold_name(){
    unsigned short moldname1 = datamap->value(QString("%1").arg(bmb_moldname1))->value.toShort();
    unsigned short moldname2 = datamap->value(QString("%1").arg(bmb_moldname2))->value.toShort();
    unsigned short moldname3 = datamap->value(QString("%1").arg(bmb_moldname3))->value.toShort();
    unsigned short moldname4 = datamap->value(QString("%1").arg(bmb_moldname4))->value.toShort();
    unsigned short moldname5 = datamap->value(QString("%1").arg(bmb_moldname5))->value.toShort();
    unsigned short moldname6 = datamap->value(QString("%1").arg(bmb_moldname6))->value.toShort();
    unsigned short moldname7 = datamap->value(QString("%1").arg(bmb_moldname7))->value.toShort();
    unsigned short moldname8 = datamap->value(QString("%1").arg(bmb_moldname8))->value.toShort();
    unsigned short moldname9 = datamap->value(QString("%1").arg(bmb_moldname9))->value.toShort();
    unsigned short moldname10 = datamap->value(QString("%1").arg(bmb_moldname10))->value.toShort();
    QByteArray mold_name;
    mold_name.append((char)(moldname1>>8));
    mold_name.append((char)(moldname1));
    mold_name.append((char)(moldname2>>8));
    mold_name.append((char)(moldname2));
    mold_name.append((char)(moldname3>>8));
    mold_name.append((char)(moldname3));
    mold_name.append((char)(moldname4>>8));
    mold_name.append((char)(moldname4));
    mold_name.append((char)(moldname5>>8));
    mold_name.append((char)(moldname5));
    mold_name.append((char)(moldname6>>8));
    mold_name.append((char)(moldname6));
    mold_name.append((char)(moldname7>>8));
    mold_name.append((char)(moldname7));
    mold_name.append((char)(moldname8>>8));
    mold_name.append((char)(moldname8));
    mold_name.append((char)(moldname9>>8));
    mold_name.append((char)(moldname9));
    mold_name.append((char)(moldname10>>8));
    mold_name.append((char)(moldname10));
    QString moldname = mold_name;
    return moldname;

}
