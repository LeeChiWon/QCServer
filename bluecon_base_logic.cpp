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
    temp_set_atnumber = addrlist.count()-1;
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

    addrlist.append(bmb_temp1_use);
    temp_onoff_atnumber = addrlist.count()-1;
    addrlist.append(bmb_temp2_use);
    addrlist.append(bmb_temp3_use);
    addrlist.append(bmb_temp4_use);
    addrlist.append(bmb_temp5_use);
    addrlist.append(bmb_temp6_use);
    addrlist.append(bmb_temp7_use);
    addrlist.append(bmb_temp8_use);
    addrlist.append(bmb_temp9_use);
    addrlist.append(bmb_temp10_use);
    addrlist.append(bmb_temp11_use);
    addrlist.append(bmb_temp12_use);
    addrlist.append(bmb_temp13_use);
    addrlist.append(bmb_temp14_use);
    addrlist.append(bmb_temp15_use);
    addrlist.append(bmb_temp16_use);

    addrlist.append(bmb_temp1_down);
    temp_down_atnumber = addrlist.count()-1;
    addrlist.append(bmb_temp2_down);
    addrlist.append(bmb_temp3_down);
    addrlist.append(bmb_temp4_down);
    addrlist.append(bmb_temp5_down);
    addrlist.append(bmb_temp6_down);
    addrlist.append(bmb_temp7_down);
    addrlist.append(bmb_temp8_down);
    addrlist.append(bmb_temp9_down);
    addrlist.append(bmb_temp10_down);
    addrlist.append(bmb_temp11_down);
    addrlist.append(bmb_temp12_down);
    addrlist.append(bmb_temp13_down);
    addrlist.append(bmb_temp14_down);
    addrlist.append(bmb_temp15_down);
    addrlist.append(bmb_temp16_down);

    addrlist.append(bmb_temp1_up);
    temp_up_atnumber = addrlist.count()-1;
    addrlist.append(bmb_temp2_up);
    addrlist.append(bmb_temp3_up);
    addrlist.append(bmb_temp4_up);
    addrlist.append(bmb_temp5_up);
    addrlist.append(bmb_temp6_up);
    addrlist.append(bmb_temp7_up);
    addrlist.append(bmb_temp8_up);
    addrlist.append(bmb_temp9_up);
    addrlist.append(bmb_temp10_up);
    addrlist.append(bmb_temp11_up);
    addrlist.append(bmb_temp12_up);
    addrlist.append(bmb_temp13_up);
    addrlist.append(bmb_temp14_up);
    addrlist.append(bmb_temp15_up);
    addrlist.append(bmb_temp16_up);

    addrlist.append(bmb_temp1_real);
    temp_real_atnumber = addrlist.count()-1;
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
    temp_real_atnumber = addrlist.count()-1;
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
    current_update();
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
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    SimpleCrypt crypto(CRYPTO_NUMBER);

    QSqlQuery mysqlquery1(remotedb);
    QString update_temp = QString("UPDATE temp_table SET ");
    QString temp_append ;
    for(int i=1;i<=8;i++){
        if(i==7){
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble();
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                               .arg(8)
                               .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_onoff_value)));

        }else if(i==8){
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;

            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6' ")
                               .arg(7)
                               .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(1)));

        }else {
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble();
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                            .arg(i)
                            .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_onoff_value)));

        }
         update_temp.append(temp_append);
    }
    temp_append = QString("WHERE machine_name=\'%1\'").arg(mancine_name);

    update_temp.append(temp_append);

    bool result = mysqlquery1.exec(update_temp);
    if(result){

    }else {
        remotedb.open();
        qDebug()<<"es600 false";
    }
    int run_mode =datamap->value(QString("%1").arg(bmb_run_mode))->value.toInt();
    int object_count = datamap->value(QString("%1").arg(bmb_object_count))->value.toInt();
    int production_count = datamap->value(QString("%1").arg(bmb_production_count))->value.toInt();
    double achievemen_rate = ((double)production_count/(double)object_count)*100;
    int cabity = datamap->value(QString("%1").arg(bmb_cabity))->value.toInt();
    int warning_flag = datamap->value(QString("%1").arg(bmb_warning_flag))->value.toInt();
    double cycle_time = datamap->value(QString("%1").arg(bmb_SHOTDATA_cycle_time))->value.toDouble()/10.0;
    QString mold_name = get_mold_name();
    QString program_name = "";
    QString warning_data;
    if(warning_flag>0){
        for(int i=0;i<warning_flag;i++){
            short temp_alaram_data = datamap->value(QString("%1").arg(bmb_alrammap1+(i*2)))->value.toInt();
            if(temp_alaram_data > 0){
                warning_data.append(QString("%1/").arg(temp_alaram_data));
            }
        }
    }else {
        warning_data="";
    }

    update_temp = QString("UPDATE Systeminfo SET production_count = '%1',"
                          "mold_name = '%2',"
                          "object_count = '%3',"
                          "cabity = '%4',"
                          "achievemen_rate = '%5',"
                          "cycle_time = \'%6\',"
                          "run_mode = \'%7\',"
                          "warning_flag = '%8',"
                          "warning_data = '%9', "
                          "machine_program = '%10' "
                          "where machine_name = \'%11\'")
                          .arg(crypto.encryptToString(QString("%1").arg(production_count)))
                          .arg(crypto.encryptToString(mold_name))
                          .arg(crypto.encryptToString(QString("%1").arg(object_count)))
                          .arg(crypto.encryptToString(QString("%1").arg(cabity)))
                          .arg(crypto.encryptToString(QString("%1").arg(achievemen_rate)))
                          .arg(crypto.encryptToString(QString("%1").arg(cycle_time,0,'f',1)))
                          .arg(crypto.encryptToString(QString("%1").arg(run_mode)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_flag)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_data)))
                          .arg(crypto.encryptToString(QString("%1").arg(program_name)))
                          .arg(mancine_name);
    mysqlquery1.exec(update_temp);

    QString S_ITEM_TYPE = crypto.encryptToString(parent_item->type->currentText());
    QString S_injstep = crypto.encryptToString(datamap->value(QString("%1").arg(bmb_injstep))->value);
    int injstep = datamap->value(QString("%1").arg(bmb_injstep))->value.toInt();
    double injVelocity[10];
    QString S_injVelocity[10];
    double injPressure[10];
    QString S_injPressure[10];
    double injPosition[10];
    QString S_injPosition[10];
    for(int i=0;i<10;i++){
        injVelocity[i] = datamap->value(QString("%1").arg(bmb_injVelocity1+i*2))->value.toDouble()/10.0;
         injPressure[i] = datamap->value(QString("%1").arg(bmb_injPressure1+i*2))->value.toDouble()/10.0;
         injPosition[i] = datamap->value(QString("%1").arg(bmb_injPosition1+i*2))->value.toDouble()/10.0;
        if(injstep<i){
            injVelocity[i] = 0.0;
            injPressure[i] = 0.0;
            injPosition[i] = 0.0;
        }
        S_injVelocity[i] = crypto.encryptToString(QString("%1").arg(injVelocity[i],0,'f',1));
        S_injPressure[i] = crypto.encryptToString(QString("%1").arg(injPressure[i],0,'f',1));
        S_injPosition[i] = crypto.encryptToString(QString("%1").arg(injPosition[i],0,'f',1));
    }
    int hldstep = datamap->value(QString("%1").arg(bmb_hldstep))->value.toInt();
    QString S_hldstep = crypto.encryptToString(datamap->value(QString("%1").arg(bmb_hldstep))->value);
    double hldPressure[5];
    QString S_hldPressure[5];
    double hldTime[5];
    QString S_hldTime[5];
    double hldVel[5];
    QString S_hldVel[5];
    for(int i=0;i<5;i++){
        hldPressure[i] = datamap->value(QString("%1").arg(bmb_hldPressure1+i*2))->value.toDouble()/10.0;
        hldTime[i] = datamap->value(QString("%1").arg(bmb_hldTime1+i*2))->value.toDouble()/10.0;
        hldVel[i] = datamap->value(QString("%1").arg(bmb_hldVel1+i*2))->value.toDouble()/10.0;
        if(hldstep<i){
            hldPressure[i] = 0.0;
            hldTime[i] = 0.0;
            hldVel[i] = 0.0;
        }
        S_hldPressure[i] = crypto.encryptToString(QString("%1").arg(hldPressure[i],0,'f',1));
        S_hldTime[i] = crypto.encryptToString(QString("%1").arg(hldTime[i],0,'f',1));
        S_hldVel[i] = crypto.encryptToString(QString("%1").arg(hldVel[i],0,'f',1));
    }

    double chgPosition[4];
    double chgSpeed[4];
    double backPressure[4];
    QString S_chgPosition[4];
    QString S_chgSpeed[4];
    QString S_backPressure[4];
    for(int i=0;i<4;i++){
        chgPosition[i] = datamap->value(QString("%1").arg(bmb_chgPosition1+i*2))->value.toDouble()/10.0;
        S_chgPosition[i] = crypto.encryptToString(QString("%1").arg(chgPosition[i],0,'f',1));
        chgSpeed[i] = datamap->value(QString("%1").arg(bmb_chgSpeed1+i*2))->value.toDouble()/10.0;
        S_chgSpeed[i] = crypto.encryptToString(QString("%1").arg(chgSpeed[i],0,'f',1));
        backPressure[i] = datamap->value(QString("%1").arg(bmb_backPressure1+i*2))->value.toDouble()/10.0;
        S_backPressure[i] = crypto.encryptToString(QString("%1").arg(backPressure[i],0,'f',1));
    }
    double suckbackPosition1 = datamap->value(QString("%1").arg(bmb_suckbackPosition1))->value.toDouble()/10.0;
    double suckbackPosition2 = datamap->value(QString("%1").arg(bmb_suckbackPosition2))->value.toDouble()/10.0;
    QString S_suckbackPosition1 = crypto.encryptToString(QString("%1").arg(suckbackPosition1,0,'f',1));
    QString S_suckbackPosition2 = crypto.encryptToString(QString("%1").arg(suckbackPosition2,0,'f',1));
    double suckbackSpeed1 = datamap->value(QString("%1").arg(bmb_suckbackSpeed1))->value.toDouble()/10.0;;
    double suckbackSpeed2 = datamap->value(QString("%1").arg(bmb_suckbackSpeed2))->value.toDouble()/10.0;;
    QString S_suckbackSpeed1  = crypto.encryptToString(QString("%1").arg(suckbackSpeed1,0,'f',1));
    QString S_suckbackSpeed2 = crypto.encryptToString(QString("%1").arg(suckbackSpeed2,0,'f',1));

    double pr_EX_Holdp = 0.0;
    QString S_pr_EX_Holdp = crypto.encryptToString(QString("%1").arg(pr_EX_Holdp,0,'f',1));

    double injtime = datamap->value(QString("%1").arg(bmb_INJETM))->value.toDouble()/10.0;
    QString S_injtime = crypto.encryptToString(QString("%1").arg(injtime,0,'f',1));

    double injdelaytime = datamap->value(QString("%1").arg(bmb_SPR2TM))->value.toDouble()/10.0;
    QString S_injdelaytime = crypto.encryptToString(QString("%1").arg(injdelaytime,0,'f',1));

    double cooltime = datamap->value(QString("%1").arg(bmb_COOLTM))->value.toDouble()/10.0;
    QString S_cooltime = crypto.encryptToString(QString("%1").arg(cooltime,0,'f',1));

    double chgdelaytime = datamap->value(QString("%1").arg(bmb_CHDLTM))->value.toDouble()/10.0;
    QString S_chgdelaytime = crypto.encryptToString(QString("%1").arg(chgdelaytime,0,'f',1));

    double sov_time  = datamap->value(QString("%1").arg(bmb_sov_time))->value.toDouble()/10.0;
    QString S_sov_time = crypto.encryptToString(QString("%1").arg(sov_time,0,'f',1));

    double sov_position  = datamap->value(QString("%1").arg(bmb_sov_position))->value.toDouble()/10.0;
    QString S_sov_positon = crypto.encryptToString(QString("%1").arg(sov_position,0,'f',1));

    update_temp = QString("UPDATE Recipe_Info "
                        "SET "
                      "ITEM_TYPE = '"+S_ITEM_TYPE+"'"
                      ",injstep = '"+S_injstep+"'"
                      ",holdstep = '"+S_hldstep+"'"
                      ",injspd_1 = '"+S_injVelocity[0]+"'"
                      ",injspd_2 = '"+S_injVelocity[1]+"'"
                      ",injspd_3 = '"+S_injVelocity[2]+"'"
                      ",injspd_4 = '"+S_injVelocity[3]+"'"
                      ",injspd_5 = '"+S_injVelocity[4]+"'"
                      ",injspd_6 = '"+S_injVelocity[5]+"'"
                      ",injspd_7 = '"+S_injVelocity[6]+"'"
                      ",injspd_8 = '"+S_injVelocity[7]+"'"
                      ",injspd_9 = '"+S_injVelocity[8]+"'"
                      ",injspd_10 = '"+S_injVelocity[9]+"'"
                      ",injprs_1 = '"+S_injPressure[0]+"'"
                      ",injprs_2 = '"+S_injPressure[1]+"'"
                      ",injprs_3 = '"+S_injPressure[2]+"'"
                      ",injprs_4 = '"+S_injPressure[3]+"'"
                      ",injprs_5 = '"+S_injPressure[4]+"'"
                      ",injprs_6 = '"+S_injPressure[5]+"'"
                      ",injprs_7 = '"+S_injPressure[6]+"'"
                      ",injprs_8 = '"+S_injPressure[7]+"'"
                      ",injprs_9 = '"+S_injPressure[8]+"'"
                      ",injprs_10 = '"+S_injPressure[9]+"'"
                      ",injpos_1 = '"+S_injPosition[0]+"'"
                      ",injpos_2 = '"+S_injPosition[1]+"'"
                      ",injpos_3 = '"+S_injPosition[2]+"'"
                      ",injpos_4 = '"+S_injPosition[3]+"'"
                      ",injpos_5 = '"+S_injPosition[4]+"'"
                      ",injpos_6 = '"+S_injPosition[5]+"'"
                      ",injpos_7 = '"+S_injPosition[6]+"'"
                      ",injpos_8 = '"+S_injPosition[7]+"'"
                      ",injpos_9 = '"+S_injPosition[8]+"'"
                      ",injpos_10 = '"+S_injPosition[9]+"'"
                      ",holdspd_1 = '"+S_hldVel[0]+"'"
                      ",holdspd_2 = '"+S_hldVel[1]+"'"
                      ",holdspd_3 = '"+S_hldVel[2]+"'"
                      ",holdspd_4 = '"+S_hldVel[3]+"'"
                      ",holdspd_5 = '"+S_hldVel[4]+"'"
                      ",holdprs_1 = '"+S_hldPressure[0]+"'"
                      ",holdprs_2 = '"+S_hldPressure[1]+"'"
                      ",holdprs_3 = '"+S_hldPressure[2]+"'"
                      ",holdprs_4 = '"+S_hldPressure[3]+"'"
                      ",holdprs_5 = '"+S_hldPressure[4]+"'"
                      ",holdtime_1 = '"+S_hldTime[0]+"'"
                      ",holdtime_2 = '"+S_hldTime[1]+"'"
                      ",holdtime_3 = '"+S_hldTime[2]+"'"
                      ",holdtime_4 = '"+S_hldTime[3]+"'"
                      ",holdtime_5 = '"+S_hldTime[4]+"'"
                      ",chgspd_1 = '"+S_chgSpeed[0]+"'"
                      ",chgspd_2 = '"+S_chgSpeed[1]+"'"
                      ",chgspd_3 = '"+S_chgSpeed[2]+"'"
                      ",chgspd_4 = '"+S_chgSpeed[3]+"'"
                      ",chgbps_1 = '"+S_backPressure[0]+"'"
                      ",chgbps_2 = '"+S_backPressure[1]+"'"
                      ",chgbps_3 = '"+S_backPressure[2]+"'"
                      ",chgbps_4 = '"+S_backPressure[3]+"'"
                      ",chgpos_1 = '"+S_chgPosition[0]+"'"
                      ",chgpos_2 = '"+S_chgPosition[1]+"'"
                      ",chgpos_3 = '"+S_chgPosition[2]+"'"
                      ",chgpos_4 = '"+S_chgPosition[3]+"'"
                      ",suckbspd_1 = '"+S_suckbackSpeed1+"'"
                      ",suckbspd_2 = '"+S_suckbackSpeed2+"'"
                      ",suckbpos_1 = '"+S_suckbackPosition1+"'"
                      ",suckbpos_2 = '"+S_suckbackPosition2+"'"
                      ",sovpos = '"+S_sov_positon+"'"
                      ",sovprs = '"+S_pr_EX_Holdp+"'"
                      ",sovtime = '"+S_sov_time+"'"
                      ",injtime = '"+S_injtime+"'"
                      ",injdeltime = '"+S_injdelaytime+"'"
                      ",cooltime = '"+S_cooltime+"'"
                      ",chgdeltime = '"+S_chgdelaytime+"' "
                );
    update_temp.append(QString("where machine_name = '%1'").arg(mancine_name));
    mysqlquery1.exec(update_temp);

}

void bluecon_base_logic::REC_save(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);

     current_shotcount = datamap->value(QString("%1").arg(bmb_production_count))->value.toInt();
    if(before_shotcount<0){
        before_shotcount = current_shotcount;
        //modbus_write_register(ctx,mb_actstatus,0);
    }
    if(before_shotcount!=current_shotcount){
        before_shotcount=current_shotcount;

         int fooldata =datamap->value(QString("%1").arg(bmb_SHOTDATA_fooldata))->value.toInt();
         double injectiontime = datamap->value(QString("%1").arg(bmb_SHOTDATA_inctiontime))->value.toDouble()/10.0;
         double fillingtime = datamap->value(QString("%1").arg(bmb_SHOTDATA_fillingtime))->value.toDouble()/10.0;
         double plasticizing_time = datamap->value(QString("%1").arg(bmb_SHOTDATA_plasticizing_time))->value.toDouble()/10.0;
         double cycle_time = datamap->value(QString("%1").arg(bmb_SHOTDATA_cycle_time))->value.toDouble()/100.0;
         double cushion_position = datamap->value(QString("%1").arg(bmb_SHOTDATA_cushion_position))->value.toDouble()/10.0;
         double clamp_close_time  = datamap->value(QString("%1").arg(bmb_SHOTDATA_clamp_close_time))->value.toDouble()/10.0;
         double plasticizing_position = datamap->value(QString("%1").arg(bmb_SHOTDATA_plasticizing_position))->value.toDouble()/10.0;
         double max_injection_speed = datamap->value(QString("%1").arg(bmb_SHOTDATA__max_injection_speed))->value.toDouble()/10.0;
         double clamp_open_position = datamap->value(QString("%1").arg(bmb_SHOTDATA_clamp_open_position))->value.toDouble()/10.0;
         double switch_over_position  = datamap->value(QString("%1").arg(bmb_SHOTDATA_switch_over_position))->value.toDouble()/10.0;
         double max_switch_over_pressure = datamap->value(QString("%1").arg(bmb_SHOTDATA__max_switch_over_pressure))->value.toDouble()/10.0;
         double max_back_pressure = datamap->value(QString("%1").arg(bmb_SHOTDATA__max_switch_over_pressure))->value.toDouble()/10.0;
         double max_screw_RPM = datamap->value(QString("%1").arg(bmb_SHOTDATA__max_screw_RPM))->value.toDouble()/10.0;
         double average_screw_RPM = datamap->value(QString("%1").arg(bmb_SHOTDATA__average_screw_RPM))->value.toDouble()/10.0;
         double max_injection_pressure = datamap->value(QString("%1").arg(bmb_SHOTDATA__max_injection_pressure))->value.toDouble()/10.0;
         double average_back_pressure = datamap->value(QString("%1").arg(bmb_SHOTDATA__average_back_pressure))->value.toDouble()/10.0;
         double temp1=datamap->value(QString("%1").arg(bmb_temp1_real))->value.toDouble()/10.0;
         double temp2=datamap->value(QString("%1").arg(bmb_temp2_real))->value.toDouble()/10.0;
         double temp3=datamap->value(QString("%1").arg(bmb_temp3_real))->value.toDouble()/10.0;
         double temp4=datamap->value(QString("%1").arg(bmb_temp4_real))->value.toDouble()/10.0;
         double temp5=datamap->value(QString("%1").arg(bmb_temp5_real))->value.toDouble()/10.0;
         double temp6=datamap->value(QString("%1").arg(bmb_temp6_real))->value.toDouble()/10.0;
         double temp7=datamap->value(QString("%1").arg(bmb_temp7_real))->value.toDouble()/10.0;
         double oil_temp=datamap->value(QString("%1").arg(bmb_temp8_real))->value.toDouble()/10.0;
         double moldtemp1 = datamap->value(QString("%1").arg(bmb_temp9_real))->value.toDouble()/10.0;
         double moldtemp2 = datamap->value(QString("%1").arg(bmb_temp10_real))->value.toDouble()/10.0;
         double moldtemp3 = datamap->value(QString("%1").arg(bmb_temp11_real))->value.toDouble()/10.0;
         double moldtemp4 = datamap->value(QString("%1").arg(bmb_temp12_real))->value.toDouble()/10.0;
         double moldtemp5 = datamap->value(QString("%1").arg(bmb_temp13_real))->value.toDouble()/10.0;
         double moldtemp6 = datamap->value(QString("%1").arg(bmb_temp14_real))->value.toDouble()/10.0;
         double moldtemp7 = datamap->value(QString("%1").arg(bmb_temp15_real))->value.toDouble()/10.0;
         double moldtemp8 = datamap->value(QString("%1").arg(bmb_temp16_real))->value.toDouble()/10.0;

         QString moldname = get_mold_name();
         QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
         QString current_time = QTime::currentTime().toString("hh:mm:ss");
         QString insertquery = QString("INSERT INTO shot_data "
                                       "(Machine_Name"
                                       ",Additional_Info_1"
                                       ",Additional_Info_2"
                                       ",TimeStamp"
                                       ",Shot_Number"
                                       ",NGmark"
                                       ",Injection_Time"
                                       ",Filling_Time"
                                       ",Plasticizing_Time"
                                       ",Cycle_Time"
                                       ",Clamp_Close_Time"
                                       ",Cushion_Position"
                                       ",Switch_Over_Position"
                                       ",Plasticizing_Position"
                                       ",Clamp_Open_Position"
                                       ",Max_Injection_Speed"
                                       ",Max_Screw_RPM"
                                       ",Average_Screw_RPM"
                                       ",Max_Injection_Pressure"
                                       ",Max_Switch_Over_Pressure"
                                       ",Max_Back_Pressure"
                                       ",Average_Back_Pressure"
                                       ",Barrel_Temperature_1"
                                       ",Barrel_Temperature_2"
                                       ",Barrel_Temperature_3"
                                       ",Barrel_Temperature_4"
                                       ",Barrel_Temperature_5"
                                       ",Barrel_Temperature_6"
                                       ",Barrel_Temperature_7"
                                       ",Hopper_Temperature"
                                       ",Mold_Temperature_1"
                                       ",Mold_Temperature_2"
                                       ",Mold_Temperature_3"
                                       ",Mold_Temperature_4"
                                       ",Mold_Temperature_5"
                                       ",Mold_Temperature_6"
                                       ",Mold_Temperature_7"
                                       ",Mold_Temperature_8)"
                                 "VALUES ("
                                       +QString("'%1'").arg(mancine_name)+","
                                       +QString("'%1'").arg(moldname)+","
                                       +"''"+","
                                       +"'"+current_date+" "+current_time+"',"
                                       +QString("%1").arg(current_shotcount)+","
                                       +QString("%1").arg(fooldata)+","
                                       +QString("%1").arg(injectiontime)+","
                                       +QString("%1").arg(fillingtime)+","
                                       +QString("%1").arg(plasticizing_time)+","
                                       +QString("%1").arg(cycle_time)+","  //Cycle_Time
                                       +QString("%1").arg(clamp_close_time)+"," //Clamp_Close_Time
                                       +QString("%1").arg(cushion_position)+"," //Cushion_Position
                                       +QString("%1").arg(switch_over_position)+","//Switch_Over_Position
                                       +QString("%1").arg(plasticizing_position)+","//Plasticizing_Position
                                       +QString("%1").arg(clamp_open_position)+","//Clamp_Open_Position
                                       +QString("%1").arg(max_injection_speed)+","//Max_Injection_Speed
                                       +QString("%1").arg(max_screw_RPM)+","//Max_Screw_RPM
                                       +QString("%1").arg(average_screw_RPM)+","//Average_Screw_RPM
                                       +QString("%1").arg(max_injection_pressure)+","//Max_Injection_Pressure
                                       +QString("%1").arg(max_switch_over_pressure)+","//Max_Switch_Over_Pressure
                                       +QString("%1").arg(max_back_pressure)+","//Max_Back_Pressure
                                       +QString("%1").arg(average_back_pressure)+","//Average_Back_Pressure
                                       +QString("%1").arg(temp1)+","
                                       +QString("%1").arg(temp2)+","
                                       +QString("%1").arg(temp3)+","
                                       +QString("%1").arg(temp4)+","
                                       +QString("%1").arg(temp5)+","
                                       +QString("%1").arg(temp6)+","
                                       +QString("%1").arg(oil_temp)+","
                                       +QString("%1").arg(temp7)+","
                                       +QString("%1").arg(moldtemp1)+","
                                       +QString("%1").arg(moldtemp2)+","
                                       +QString("%1").arg(moldtemp3)+","
                                       +QString("%1").arg(moldtemp4)+","
                                       +QString("%1").arg(moldtemp5)+","
                                       +QString("%1").arg(moldtemp6)+","
                                       +QString("%1").arg(moldtemp7)+","
                                       +QString("%1").arg(moldtemp8)+")"
                    );
         queryresult = mysqlquery1.exec(insertquery);

         QString Inj_Velocitystr;
         QString injPressurestr;
         QString injPositionstr;
         int injstep = datamap->value(QString("%1").arg(mb_injstep))->value.toInt();
         double injVelocity[10];
         double injPressure[10];
         double injPosition[10];
         for(int i=0;i<10;i++){
             if(i==9){
                 injVelocity[i] = datamap->value(QString("%1").arg(bmb_injVelocity1+i*2))->value.toDouble()/100.0;
                 injPressure[i] = datamap->value(QString("%1").arg(bmb_injPressure1+i*2))->value.toDouble();
                 injPosition[i] = datamap->value(QString("%1").arg(bmb_injPosition1+i*2))->value.toDouble()/10.0;
                 if(injstep>i){
                     Inj_Velocitystr.append(QString("%1").arg(injVelocity[i]));
                     injPressurestr.append(QString("%1").arg(injPressure[i]));
                     injPositionstr.append(QString("%1").arg(injPosition[i]));
                 }else{
                     Inj_Velocitystr.append(QString("%1").arg(0.0));
                     injVelocity[i] = 0.0;
                     injPressurestr.append(QString("%1").arg(0.0));
                     injPressure[i] = 0.0;
                     injPositionstr.append(QString("%1").arg(0.0));
                     injPosition[i] = 0.0;
                 }
             }else {
                 injVelocity[i] = datamap->value(QString("%1").arg(bmb_injVelocity1+i*2))->value.toDouble()/100.0;
                 injPressure[i] = datamap->value(QString("%1").arg(bmb_injPressure1+i*2))->value.toDouble();
                 injPosition[i] = datamap->value(QString("%1").arg(bmb_injPosition1+i*2))->value.toDouble()/10.0;
                 if(injstep>i){
                     Inj_Velocitystr.append(QString("%1/").arg(injVelocity[i]));
                     injPressurestr.append(QString("%1/").arg(injPressure[i]));
                     injPositionstr.append(QString("%1/").arg(injPosition[i]));
                 }else{
                     Inj_Velocitystr.append(QString("%1/").arg(0.0));
                     injVelocity[i] = 0.0;
                     injPressurestr.append(QString("%1/").arg(0.0));
                     injPressure[i] = 0.0;
                     injPositionstr.append(QString("%1/").arg(0.0));
                     injPosition[i] = 0.0;
                 }
             }
         }
        QString hldPressurestr;
        QString hldTimestr;
        QString hldVelstr;
        double hldTime[3];
        double hldPressure[4];
        double hldVel[3];
        int hldstep = datamap->value(QString("%1").arg(mb_hldstep))->value.toInt();
        for(int i=0;i<5;i++){
            if(i == 4){
                 hldPressure[i] = datamap->value(QString("%1").arg(bmb_hldPressure1+i*2))->value.toDouble()/10.0;
                 hldTime[i] = datamap->value(QString("%1").arg(bmb_hldTime1+i*2))->value.toDouble()/10.0;
                  hldVel[i] = datamap->value(QString("%1").arg(bmb_hldVel1+i*2))->value.toDouble()/10.0;
                if(hldstep>i){
                    hldPressurestr.append(QString("%1").arg(hldPressure[i]));
                    hldTimestr.append(QString("%1").arg(hldTime[i]));
                    hldVelstr.append(QString("%1").arg(hldVel[0]));
                }else{
                    hldPressurestr.append(QString("%1").arg(0.0));
                    hldPressure[i] = 0.0;
                    hldTimestr.append(QString("%1").arg(0.0));
                    hldTime[i] = 0.0;
                    hldVelstr.append(QString("%1").arg(0.0));
                }
            }else {
                 hldPressure[i] = datamap->value(QString("%1").arg(bmb_hldPressure1+i*2))->value.toDouble()/10.0;
                 hldTime[i] = datamap->value(QString("%1").arg(bmb_hldTime1+i*2))->value.toDouble()/10.0;
                 hldVel[i] = datamap->value(QString("%1").arg(bmb_hldVel1+i*2))->value.toDouble()/10.0;
                if(hldstep>i){
                    hldPressurestr.append(QString("%1/").arg(hldPressure[i]));
                    hldTimestr.append(QString("%1/").arg(hldTime[i]));
                    hldVelstr.append(QString("%1/").arg(hldVel[i]));
                }else{
                    hldPressurestr.append(QString("%1/").arg(0.0));
                    hldPressure[i] = 0.0;
                    hldTimestr.append(QString("%1/").arg(0.0));
                    hldTime[i] = 0.0;
                    hldVelstr.append(QString("%1/").arg(0.0));
                    hldVel[i] = 0.0;
                }
            }
        }
        QString chgPositionstr;
        double chgPosition[4];
        for(int i=0;i<4;i++){
            chgPosition[i] = datamap->value(QString("%1").arg(bmb_chgPosition1+i*2))->value.toDouble()/10.0;
        }
        chgPositionstr = QString("%1/%2/%3/%4").arg(chgPosition[0]).arg(chgPosition[1])
                                               .arg(chgPosition[2]).arg(chgPosition[3]);
        QString chgSpeedstr;
        double chgSpeed[4];
        for(int i=0;i<4;i++){
            chgSpeed[i] = datamap->value(QString("%1").arg(mb_chgSpeed1+i*2))->value.toDouble()/10.0;
        }
        chgSpeedstr = QString("%1/%2/%3/%4").arg(chgSpeed[0]).arg(chgSpeed[1])
                                               .arg(chgSpeed[2]).arg(chgSpeed[3]);
        QString backPressurestr;
        double backPressure[4];
        for(int i=0;i<4;i++){
            backPressure[i] = datamap->value(QString("%1").arg(bmb_backPressure1+i*2))->value.toDouble()/10.0;
        }
        backPressurestr = QString("%1/%2/%3/%4").arg(backPressure[0]).arg(backPressure[1])
                                               .arg(backPressure[2]).arg(backPressure[3]);
        QString suckbackPositionstr;
        double suckbackPosition1 = datamap->value(QString("%1").arg(bmb_suckbackPosition1))->value.toDouble()/10.0;
        double suckbackPosition2 = datamap->value(QString("%1").arg(bmb_suckbackPosition2))->value.toDouble()/10.0;
        suckbackPositionstr = QString("%1/%2").arg(suckbackPosition1).arg(suckbackPosition2);

        QString suckbackSpeedstr;
        double suckbackSpeed1 = datamap->value(QString("%1").arg(bmb_suckbackSpeed1))->value.toDouble()/100.0;;
        double suckbackSpeed2 = datamap->value(QString("%1").arg(bmb_suckbackSpeed2))->value.toDouble()/100.0;;
        suckbackSpeedstr = QString("%1/%2").arg(suckbackSpeed1).arg(suckbackSpeed2);

        double tempues[8];
        for(int i=0;i<8;i++){
            tempues[i] = datamap->value(QString("%1").arg(bmb_temp1_use+i))->value.toDouble()/10.0;
        }

        QString tempsetstr;
        double tempset[8];
        for(int i=0;i<8;i++){
            tempset[i] = datamap->value(QString("%1").arg(bmb_temp1_set+i*2))->value.toDouble()/10.0;
        }
        if(tempues[0]){
            tempsetstr.append(QString("%1/").arg(tempset[0]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[0] = 0.0;
        }
        if(tempues[1]){
            tempsetstr.append(QString("%1/").arg(tempset[1]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[1] = 0.0;
        }
        if(tempues[2]){
            tempsetstr.append(QString("%1/").arg(tempset[2]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[2] = 0.0;
        }
        if(tempues[3]){
            tempsetstr.append(QString("%1/").arg(tempset[3]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[3] = 0.0;
        }
        if(tempues[4]){
            tempsetstr.append(QString("%1/").arg(tempset[4]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[4] = 0.0;
        }
        if(tempues[5]){
            tempsetstr.append(QString("%1/").arg(tempset[5]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[5] = 0.0;
        }
        //oil
         tempsetstr.append(QString("%1/").arg(tempset[7]));

        if(tempues[6]){ //hopper
            tempsetstr.append(QString("%1/").arg(tempset[6]));
        }else{
            tempsetstr.append(QString("%1/").arg(0.0));
            tempset[6] = 0.0;
        }

        int moldtempuse[8];
        for(int i=0;i<8;i++){
            moldtempuse[i] = datamap->value(QString("%1").arg(bmb_temp9_use+i))->value.toInt();
        }
        QString moldtempstr;
        double moldtempset[8];
        for(int i=0;i<8;i++){
            moldtempset[i] = datamap->value(QString("%1").arg(bmb_temp9_set+i*2))->value.toDouble()/10.0;
        }
        if(moldtempuse[0]){
            moldtempstr.append(QString("%1/").arg(moldtempset[0]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[0] = 0.0;
        }
        if(moldtempuse[1]){
            moldtempstr.append(QString("%1/").arg(moldtempset[1]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[1] = 0.0;
        }
        if(moldtempuse[2]){
            moldtempstr.append(QString("%1/").arg(moldtempset[2]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[2] = 0.0;
        }
        if(moldtempuse[3]){
            moldtempstr.append(QString("%1/").arg(moldtempset[3]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[3] = 0.0;
        }
        if(moldtempuse[4]){
            moldtempstr.append(QString("%1/").arg(moldtempset[4]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[4] = 0.0;
        }
        if(moldtempuse[5]){
            moldtempstr.append(QString("%1/").arg(moldtempset[5]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[5] = 0.0;
        }
        if(moldtempuse[6]){
            moldtempstr.append(QString("%1/").arg(moldtempset[6]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
            moldtempset[6] = 0.0;
        }
        if(moldtempuse[7]){
            moldtempstr.append(QString("%1").arg(moldtempset[7]));
        }else{
            moldtempstr.append(QString("%1").arg(0.0));
            moldtempset[7] = 0.0;
        }
        QString timerstr;
        double injtime = datamap->value(QString("%1").arg(bmb_SPR2TM))->value.toDouble()/10.0;
        double cooltime = datamap->value(QString("%1").arg(bmb_COOLTM))->value.toDouble()/10.0;
        double chgtime = datamap->value(QString("%1").arg(bmb_CHDLTM))->value.toDouble()/10.0;
        double injdelaytime = datamap->value(QString("%1").arg(bmb_INJETM))->value.toDouble()/10.0;
        timerstr = QString("%1/%2/%3/%4").arg(injtime).arg(cooltime).arg(injdelaytime).arg(chgtime);

        double sov_time  = datamap->value(QString("%1").arg(bmb_sov_time))->value.toDouble()/10.0;
        double sov_position  = datamap->value(QString("%1").arg(bmb_sov_position))->value.toDouble()/10.0;

        insertquery =QString("INSERT INTO shot_data_rec"
                "(Machine_Name"
                ",Additional_Info_1"
                ",Additional_Info_2"
                ",TimeStamp"
                ",Shot_Number"
                ",Inj_Velocity"
                ",Inj_Pressure"
                ",Inj_Position"
                ",SOV_Time"
                ",SOV_Position"
                ",Hld_Pressure"
                ",Hld_Time"
                ",Hld_Vel"
                ",Chg_Position"
                ",Chg_Speed"
                ",BackPressure"
                ",Suckback_Position"
                ",Suckback_Speed"
                ",Barrel_Temperature"
                ",Mold_Temperature"
                ",Timer)"
          "VALUES"
                "("+QString("'%1'").arg(mancine_name)+","
                +QString("'%1'").arg(moldname)+","
                +"''"+","
                +"'"+current_date+" "+current_time+"',"
                +QString("%1").arg(current_shotcount)+","
                +"\'"+Inj_Velocitystr+"\'"+","
                +"\'"+injPressurestr+"\'"+","
                +"\'"+injPositionstr+"\'"+","
                +QString("%1").arg(sov_time,0,'f',1)+","
                +QString("%1").arg(sov_position,0,'f',1)+","
                +"\'"+hldPressurestr+"\'"+","
                +"\'"+hldTimestr+"\'"+","
                +"\'"+hldVelstr+"\'"+","
                +"\'"+chgPositionstr+"\'"+","
                +"\'"+chgSpeedstr+"\'"+","
                +"\'"+backPressurestr+"\'"+","
                +"\'"+suckbackPositionstr+"\'"+","
                +"\'"+suckbackSpeedstr+"\'"+","
                +"\'"+tempsetstr+"\'"+","
                +"\'"+moldtempstr+"\'"+","
                +"\'"+timerstr+"\'"+")"
             );

        queryresult = mysqlquery1.exec(insertquery);
//        qDebug()<<mysqlquery1.lastQuery();

        qDebug()<<mysqlquery1.lastQuery();
        qDebug()<<mysqlquery1.lastError().text();

        mysqlquery1.exec("INSERT INTO shot_data_rec2"
                         "(Machine_Name"
                         ",Additional_Info_1"
                         ",Additional_Info_2"
                         ",TimeStamp"
                         ",Shot_Number"
                         ",inj_step"
                         ",hold_step"
                         ",Inj_Velocity_1"
                         ",Inj_Velocity_2"
                         ",Inj_Velocity_3"
                         ",Inj_Velocity_4"
                         ",Inj_Velocity_5"
                         ",Inj_Velocity_6"
                         ",Inj_Velocity_7"
                         ",Inj_Velocity_8"
                         ",Inj_Velocity_9"
                         ",Inj_Velocity_10"
                         ",Inj_Pressure_1"
                         ",Inj_Pressure_2"
                         ",Inj_Pressure_3"
                         ",Inj_Pressure_4"
                         ",Inj_Pressure_5"
                         ",Inj_Pressure_6"
                         ",Inj_Pressure_7"
                         ",Inj_Pressure_8"
                         ",Inj_Pressure_9"
                         ",Inj_Pressure_10"
                         ",Inj_Position_1"
                         ",Inj_Position_2"
                         ",Inj_Position_3"
                         ",Inj_Position_4"
                         ",Inj_Position_5"
                         ",Inj_Position_6"
                         ",Inj_Position_7"
                         ",Inj_Position_8"
                         ",Inj_Position_9"
                         ",Inj_Position_10"
                         ",SOV_Time"
                         ",SOV_Position"
                         ",SOV_Prs"
                         ",Hld_Pressure_1"
                         ",Hld_Pressure_2"
                         ",Hld_Pressure_3"
                         ",Hld_Pressure_4"
                         ",Hld_Pressure_5"
                         ",Hld_Time_1"
                         ",Hld_Time_2"
                         ",Hld_Time_3"
                         ",Hld_Time_4"
                         ",Hld_Time_5"
                         ",Hld_Vel_1"
                         ",Hld_Vel_2"
                         ",Hld_Vel_3"
                         ",Hld_Vel_4"
                         ",Hld_Vel_5"
                         ",Chg_Position_1"
                         ",Chg_Position_2"
                         ",Chg_Position_3"
                         ",Chg_Position_4"
                         ",Chg_Speed_1"
                         ",Chg_Speed_2"
                         ",Chg_Speed_3"
                         ",Chg_Speed_4"
                         ",BackPressure_1"
                         ",BackPressure_2"
                         ",BackPressure_3"
                         ",BackPressure_4"
                         ",Suckback_Position_1"
                         ",Suckback_Position_2"
                         ",Suckback_Speed_1"
                         ",Suckback_Speed_2"
                         ",Barrel_Temperature_1"
                         ",Barrel_Temperature_2"
                         ",Barrel_Temperature_3"
                         ",Barrel_Temperature_4"
                         ",Barrel_Temperature_5"
                         ",Barrel_Temperature_6"
                         ",Barrel_Temperature_7"
                         ",Barrel_Temperature_hopper"
                         ",Mold_Temperature_1"
                         ",Mold_Temperature_2"
                         ",Mold_Temperature_3"
                         ",Mold_Temperature_4"
                         ",Mold_Temperature_5"
                         ",Mold_Temperature_6"
                         ",Mold_Temperature_7"
                         ",Mold_Temperature_8"
                         ",Mold_Temperature_9"
                         ",Mold_Temperature_10"
                         ",Mold_Temperature_11"
                         ",Mold_Temperature_12"
                         ",set_injtime"
                         ",set_cooltime"
                         ",set_injdelaytime"
                         ",set_chgdelaytime)"
                   "VALUES"
                         "("+QString("'%1'").arg(mancine_name)+","
                         ""+QString("'%1'").arg(moldname)+","
                         ""+"''"+","
                         ""+"'"+current_date+" "+current_time+"',"
                         ""+QString("%1").arg(current_shotcount)+","
                         ""+QString("%1").arg(injstep)+","
                         ""+QString("%1").arg(hldstep)+","
                         ""+QString("%1").arg(injVelocity[0],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[1],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[2],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[3],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[4],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[5],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[6],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[7],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[8],0,'f',1)+","
                         ""+QString("%1").arg(injVelocity[9],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[0],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[1],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[2],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[3],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[4],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[5],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[6],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[7],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[8],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[9],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[0],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[1],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[2],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[3],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[4],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[5],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[6],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[7],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[8],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[9],0,'f',1)+","
                         ""+QString("%1").arg(sov_time,0,'f',1)+","
                         ""+QString("%1").arg(sov_position,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[0],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[1],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[2],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[3],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[4],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[0],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[1],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[2],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[3],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[4],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[0],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[1],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[2],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[3],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[4],0,'f',1)+","
                         ""+QString("%1").arg(chgPosition[0],0,'f',1)+","
                         ""+QString("%1").arg(chgPosition[1],0,'f',1)+","
                         ""+QString("%1").arg(chgPosition[2],0,'f',1)+","
                         ""+QString("%1").arg(chgPosition[3],0,'f',1)+","
                         ""+QString("%1").arg(chgSpeed[0],0,'f',1)+","
                         ""+QString("%1").arg(chgSpeed[1],0,'f',1)+","
                         ""+QString("%1").arg(chgSpeed[2],0,'f',1)+","
                         ""+QString("%1").arg(chgSpeed[3],0,'f',1)+","
                         ""+QString("%1").arg(backPressure[0],0,'f',1)+","
                         ""+QString("%1").arg(backPressure[1],0,'f',1)+","
                         ""+QString("%1").arg(backPressure[2],0,'f',1)+","
                         ""+QString("%1").arg(backPressure[3],0,'f',1)+","
                         ""+QString("%1").arg(suckbackPosition1,0,'f',1)+","
                         ""+QString("%1").arg(suckbackPosition2,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed1,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed2,0,'f',1)+","
                         ""+QString("%1").arg(tempset[0],0,'f',1)+","
                         ""+QString("%1").arg(tempset[1],0,'f',1)+","
                         ""+QString("%1").arg(tempset[2],0,'f',1)+","
                         ""+QString("%1").arg(tempset[3],0,'f',1)+","
                         ""+QString("%1").arg(tempset[4],0,'f',1)+","
                         ""+QString("%1").arg(tempset[5],0,'f',1)+","
                         ""+QString("%1").arg(tempset[7],0,'f',1)+","
                         ""+QString("%1").arg(tempset[6],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[0],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[1],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[2],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[3],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[4],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[5],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[6],0,'f',1)+","
                         ""+QString("%1").arg(moldtempset[7],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(injtime,0,'f',1)+","
                         ""+QString("%1").arg(cooltime,0,'f',1)+","
                         ""+QString("%1").arg(injdelaytime,0,'f',1)+","
                         ""+QString("%1").arg(chgtime,0,'f',1)+")"
               );

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
