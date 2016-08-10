#include "es600_base_locgic.h"
#include "mslotitem.h"
#include <QSqlError>

es600_base_locgic::es600_base_locgic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
    before_shotcount =-1;
    current_shotcount =-1;
}
bool es600_base_locgic::init(){
     mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
     datamap = new QMap<QString,es600value *>;
     alrammap = new QMap<QString,alrammap_data *>;
     for(int i=0;i<100;i++){
         alrammap->insert(QString("%1").arg(i),new alrammap_data());
     }
     ip = parent_item->ip->text();
     litedb = QSqlDatabase::database("localdb");
     QSqlQuery litequery1(litedb);
     litequery1.exec("select * from systemset;");
     litequery1.next();
     if(litequery1.value("remoteservertype").toString().compare("MYSQL")==0){
         remotedb = QSqlDatabase::addDatabase("QMYSQL",parent_item->iptext);
         typeDB = MYSQL;
     }else if(litequery1.value("remoteservertype").toString().compare("ODBC")==0){
         remotedb = QSqlDatabase::addDatabase("QODBC",parent_item->iptext);
         typeDB = ODBC;
     }
     remotedb.setHostName(litequery1.value("remoteserverip").toString());
     remotedb.setDatabaseName(litequery1.value("remoteserverdbname").toString());
     remotedb.setPort(litequery1.value("remoteserverport").toInt());
     remotedb.setUserName(litequery1.value("remoteserverusername").toString());
     remotedb.setPassword(litequery1.value("remoteserveruserpassword").toString());

     if(!remotedb.open()){
         qDebug()<<"es600 DB not open";

     }else {
        qDebug()<<"es600 DB open";
     }

     qctx = new QModbusTcpClient(this);
     qctx->setConnectionParameter(QModbusDevice::NetworkAddressParameter,parent_item->iptext);
     qctx->setConnectionParameter(QModbusDevice::NetworkPortParameter,502);

     qctx->setTimeout(3000);

     if(!qctx->connectDevice()){
         qDebug()<<"es600 qctx connect false";
     }else {

     }

     connect(qctx,&QModbusClient::stateChanged,this,&es600_base_locgic::modbusstatue_change);


     addrlist.append(mb_object_count);
     addrlist.append(mb_production_count);
     addrlist.append(mb_run_mode);
     addrlist.append(mb_warning_flag);
     addrlist.append(mb_cabity);

     addrlist.append(mb_temp1_set);
     temp_set_atnumber = addrlist.count()-1;
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
     temp_up_atnumber = addrlist.count()-1;
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

     addrlist.append(mb_temp1_down);
     temp_down_atnumber = addrlist.count()-1;
     addrlist.append(mb_temp2_down);
     addrlist.append(mb_temp3_down);
     addrlist.append(mb_temp4_down);
     addrlist.append(mb_temp5_down);
     addrlist.append(mb_temp6_down);
     addrlist.append(mb_temp7_down);
     addrlist.append(mb_temp8_down);
     addrlist.append(mb_temp9_down);
     addrlist.append(mb_temp10_down);
     addrlist.append(mb_temp11_down);
     addrlist.append(mb_temp12_down);
     addrlist.append(mb_temp13_down);
     addrlist.append(mb_temp14_down);
     addrlist.append(mb_temp15_down);
     addrlist.append(mb_temp16_down);

     addrlist.append(mb_temp1_real);
     temp_real_atnumber = addrlist.count()-1;
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

     addrlist.append(mb_temp1_onoff);
     temp_onoff_atnumber = addrlist.count()-1;
     addrlist.append(mb_temp2_onoff);
     addrlist.append(mb_temp3_onoff);
     addrlist.append(mb_temp4_onoff);
     addrlist.append(mb_temp5_onoff);
     addrlist.append(mb_temp6_onoff);
     addrlist.append(mb_temp7_onoff);
     addrlist.append(mb_temp8_onoff);
     addrlist.append(mb_temp9_onoff);
     addrlist.append(mb_temp10_onoff);
     addrlist.append(mb_temp11_onoff);
     addrlist.append(mb_temp12_onoff);
     addrlist.append(mb_temp13_onoff);
     addrlist.append(mb_temp14_onoff);
     addrlist.append(mb_temp15_onoff);
     addrlist.append(mb_temp16_onoff);

     addrlist.append(mb_actstatus);

     addrlist.append(mb_SHOTDATA_count);
     addrlist.append(mb_currenttotalcount);
     addrlist.append(mb_SHOTDATA_fooldata);
     addrlist.append(mb_SHOTDATA_fillingtime);
     addrlist.append(mb_SHOTDATA_plasticizing_time);
     addrlist.append(mb_SHOTDATA_cycle_time);
     addrlist.append(mb_SHOTDATA_cushion_position);
     addrlist.append(mb_SHOTDATA_plasticizing_position);
     addrlist.append(mb_SHOTDATA_temp1);
     addrlist.append(mb_SHOTDATA_temp2);
     addrlist.append(mb_SHOTDATA_temp3);
     addrlist.append(mb_SHOTDATA_temp4);
     addrlist.append(mb_SHOTDATA_temp5);
     addrlist.append(mb_SHOTDATA_temp6);
     addrlist.append(mb_SHOTDATA_temp7);
     addrlist.append(mb_SHOTDATA_temp8);
     addrlist.append(mb_SHOTDATA_oil_temp);
     addrlist.append(mb_moldname1);
     addrlist.append(mb_moldname2);
     addrlist.append(mb_moldname3);
     addrlist.append(mb_moldname4);
     addrlist.append(mb_moldname5);

     addrlist.append(mb_injstep);
     addrlist.append(mb_hldstep);

     addrlist.append(mb_injVelocity1);
     addrlist.append(mb_injVelocity2);
     addrlist.append(mb_injVelocity3);
     addrlist.append(mb_injVelocity4);
     addrlist.append(mb_injVelocity5);
     addrlist.append(mb_injVelocity6);
     addrlist.append(mb_injPressure1);
     addrlist.append(mb_injPressure2);
     addrlist.append(mb_injPressure3);
     addrlist.append(mb_injPressure4);
     addrlist.append(mb_injPressure5);
     addrlist.append(mb_injPressure6);
     addrlist.append(mb_injPosition1);
     addrlist.append(mb_injPosition2);
     addrlist.append(mb_injPosition3);
     addrlist.append(mb_injPosition4);
     addrlist.append(mb_injPosition5);
     addrlist.append(mb_injPosition6);

     addrlist.append(mb_injstep);
     addrlist.append(mb_hldstep);

     addrlist.append(mb_hldPressure1);
     addrlist.append(mb_hldPressure2);
     addrlist.append(mb_hldPressure3);

     addrlist.append(mb_hldTime1);
     addrlist.append(mb_hldTime2);
     addrlist.append(mb_hldTime3);

     addrlist.append(mb_hldVel1);
     addrlist.append(mb_hldVel2);
     addrlist.append(mb_hldVel3);

     addrlist.append(mb_chgPosition1);
     addrlist.append(mb_chgPosition2);
     addrlist.append(mb_chgPosition3);
     addrlist.append(mb_chgPosition4);

     addrlist.append(mb_chgSpeed1);
     addrlist.append(mb_chgSpeed2);
     addrlist.append(mb_chgSpeed3);
     addrlist.append(mb_chgSpeed4);

     addrlist.append(mb_backPressure1);
     addrlist.append(mb_backPressure2);
     addrlist.append(mb_backPressure3);
     addrlist.append(mb_backPressure4);

     addrlist.append(mb_suckbackPosition2);

     addrlist.append(mb_suckbackSpeed1);
     addrlist.append(mb_suckbackSpeed2);

     addrlist.append(mb_tempues1);
     addrlist.append(mb_tempues2);
     addrlist.append(mb_tempues3);
     addrlist.append(mb_tempues4);
     addrlist.append(mb_tempues5);
     addrlist.append(mb_tempues6);
     addrlist.append(mb_tempues7);

     addrlist.append(mb_tempset1);
     addrlist.append(mb_tempset2);
     addrlist.append(mb_tempset3);
     addrlist.append(mb_tempset4);
     addrlist.append(mb_tempset5);
     addrlist.append(mb_tempset6);
     addrlist.append(mb_tempset7);
     addrlist.append(mb_oilset);

     addrlist.append(mb_moldtempuse1);
     addrlist.append(mb_moldtempuse2);
     addrlist.append(mb_moldtempuse3);
     addrlist.append(mb_moldtempuse4);
     addrlist.append(mb_moldtempuse5);
     addrlist.append(mb_moldtempuse6);
     addrlist.append(mb_moldtempuse7);
     addrlist.append(mb_moldtempuse8);

    addrlist.append(mb_moldtempset1);
    addrlist.append(mb_moldtempset2);
    addrlist.append(mb_moldtempset3);
    addrlist.append(mb_moldtempset4);
    addrlist.append(mb_moldtempset5);
    addrlist.append(mb_moldtempset6);
    addrlist.append(mb_moldtempset7);
    addrlist.append(mb_moldtempset8);

    addrlist.append(mb_injtime);
    addrlist.append(mb_cooltime);
    addrlist.append(mb_chgtime);

    addrlist.append(mb_alrammap1);
    addrlist.append(mb_alrammap2);
    addrlist.append(mb_alrammap3);
    addrlist.append(mb_alrammap4);
    addrlist.append(mb_alrammap5);
    addrlist.append(mb_alrammap6);
    addrlist.append(mb_alrammap7);
    addrlist.append(mb_alrammap8);
    addrlist.append(mb_alrammap9);
    addrlist.append(mb_alrammap10);


     modbus_thread = new es600_modbus_thread(this);
     modbus_thread->start();

     initflag=true;
     return initflag;

}
void es600_base_locgic::loop(){
    if(qctx->state()==QModbusDevice::ConnectedState){
        QModbusReply *reply;
        for(int i=0;i<addrlist.size();i++){
            reply = qctx->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,addrlist.at(i),1),1);
            connect(reply,SIGNAL(finished()),this,SLOT(modbusread_ready()));
        }
    }
}
//es600_modbus_thread 에서 현재 함수를 호출한다.
void es600_base_locgic::es600_base_loop(){

    TB_current_update();

    TB_REC_save();

    alram_update();

}
void es600_base_locgic::TB_current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    SimpleCrypt crypto(CRYPTO_NUMBER);

    QSqlQuery mysqlquery1(remotedb);
    QString update_temp = QString("UPDATE temp_table SET ");
    QString temp_append ;
    for(int i=1;i<=16;i++){
        if(i == 16){
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble()/10.0;
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6' ")
                               .arg(i)
                               .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_onoff_value,0,'f',1)));

        }else if(i==7){
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble()/10.0;
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                               .arg(8)
                               .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(1)));

        }else if(i==8){
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble()/10.0;
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                               .arg(7)
                               .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temp_onoff_value,0,'f',1)));

        }else {
            double temp_set_value = datamap->value(QString("%1").arg(addrlist.at(temp_set_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_up_value = datamap->value(QString("%1").arg(addrlist.at(temp_up_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_down_value = datamap->value(QString("%1").arg(addrlist.at(temp_down_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_real_value = datamap->value(QString("%1").arg(addrlist.at(temp_real_atnumber+i-1)))->value.toDouble()/10.0;
            double temp_onoff_value = datamap->value(QString("%1").arg(addrlist.at(temp_onoff_atnumber+i-1)))->value.toDouble()/10.0;
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                            .arg(i)
                            .arg(crypto.encryptToString(QString("%1").arg(temp_set_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_up_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_down_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_real_value,0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(temp_onoff_value,0,'f',1)));

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
    int run_mode =datamap->value(QString("%1").arg(mb_run_mode))->value.toInt();
    int object_count = datamap->value(QString("%1").arg(mb_object_count))->value.toInt();
    int production_count = datamap->value(QString("%1").arg(mb_production_count))->value.toInt();
    double achievemen_rate = (double)object_count/(double)production_count;
    int cabity = datamap->value(QString("%1").arg(mb_cabity))->value.toInt();
    int warning_flag = datamap->value(QString("%1").arg(mb_warning_flag))->value.toInt();
    double cycle_time = datamap->value(QString("%1").arg(mb_SHOTDATA_cycle_time))->value.toDouble()/100.0;
    QString mold_name = get_moldname();
    QString program_name = "";
    QString warning_data;
    if(warning_flag>0){
        for(int i=0;i<10;i++){
            short temp_alaram_data = datamap->value(QString("%1").arg(mb_alrammap1+(i*2)))->value.toInt();
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
    QString S_injstep = crypto.encryptToString(datamap->value(QString("%1").arg(mb_injstep))->value);
    int injstep = datamap->value(QString("%1").arg(mb_injstep))->value.toInt();
    double injVelocity[6];
    QString S_injVelocity[6];
    double injPressure[6];
    QString S_injPressure[6];
    double injPosition[6];
    QString S_injPosition[6];
    for(int i=0;i<6;i++){
        injVelocity[i] = datamap->value(QString("%1").arg(mb_injVelocity1+i*2))->value.toDouble()/10.0;
         injPressure[i] = datamap->value(QString("%1").arg(mb_injPressure1+i*2))->value.toDouble()/10.0;
         injPosition[i] = datamap->value(QString("%1").arg(mb_injPosition1+i*2))->value.toDouble()/10.0;
        if(injstep<i){
            injVelocity[i] = 0.0;
            injPressure[i] = 0.0;
            injPosition[i] = 0.0;
        }
        S_injVelocity[i] = crypto.encryptToString(QString("%1").arg(injVelocity[i],0,'f',1));
        S_injPressure[i] = crypto.encryptToString(QString("%1").arg(injPressure[i],0,'f',1));
        S_injPosition[i] = crypto.encryptToString(QString("%1").arg(injPosition[i],0,'f',1));
    }
    int hldstep = datamap->value(QString("%1").arg(mb_hldstep))->value.toInt();
    QString S_hldstep = crypto.encryptToString(datamap->value(QString("%1").arg(mb_hldstep))->value);
    double hldPressure[3];
    QString S_hldPressure[3];
    double hldTime[3];
    QString S_hldTime[3];
    double hldVel[3];
    QString S_hldVel[3];
    for(int i=0;i<3;i++){
        hldPressure[i] = datamap->value(QString("%1").arg(mb_hldPressure1+i*2))->value.toDouble()/10.0;
        hldTime[i] = datamap->value(QString("%1").arg(mb_hldTime1+i*2))->value.toDouble()/10.0;
        hldVel[i] = datamap->value(QString("%1").arg(mb_hldVel1+i*2))->value.toDouble()/10.0;
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
        chgPosition[i] = datamap->value(QString("%1").arg(mb_chgPosition1+i*2))->value.toDouble()/10.0;
        S_chgPosition[i] = crypto.encryptToString(QString("%1").arg(chgPosition[i],0,'f',1));
        chgSpeed[i] = datamap->value(QString("%1").arg(mb_chgSpeed1+i*2))->value.toDouble()/10.0;
        S_chgSpeed[i] = crypto.encryptToString(QString("%1").arg(chgSpeed[i],0,'f',1));
        backPressure[i] = datamap->value(QString("%1").arg(mb_backPressure1+i*2))->value.toDouble()/10.0;
        S_backPressure[i] = crypto.encryptToString(QString("%1").arg(backPressure[i],0,'f',1));
    }
    double suckbackPosition0 = 0.0;
    double suckbackPosition2 = datamap->value(QString("%1").arg(mb_suckbackPosition2))->value.toDouble()/10.0;
    QString S_suckbackPosition0 = crypto.encryptToString(QString("%1").arg(suckbackPosition0,0,'f',1));
    QString S_suckbackPosition2 = crypto.encryptToString(QString("%1").arg(suckbackPosition2,0,'f',1));
    double suckbackSpeed1 = datamap->value(QString("%1").arg(mb_suckbackSpeed1))->value.toDouble()/10.0;;
    double suckbackSpeed2 = datamap->value(QString("%1").arg(mb_suckbackSpeed2))->value.toDouble()/10.0;;
    QString S_suckbackSpeed1  = crypto.encryptToString(QString("%1").arg(suckbackSpeed1,0,'f',1));
    QString S_suckbackSpeed2 = crypto.encryptToString(QString("%1").arg(suckbackSpeed2,0,'f',1));

    double pr_EX_Holdp = 0.0;
    QString S_pr_EX_Holdp = crypto.encryptToString(QString("%1").arg(pr_EX_Holdp,0,'f',1));

    double injtime = datamap->value(QString("%1").arg(mb_injtime))->value.toDouble()/100.0;
    QString S_injtime = crypto.encryptToString(QString("%1").arg(injtime,0,'f',1));

    double injdelaytime = 0.0;
    QString S_injdelaytime = crypto.encryptToString(QString("%1").arg(injdelaytime,0,'f',1));

    double cooltime = datamap->value(QString("%1").arg(mb_cooltime))->value.toDouble()/100.0;
    QString S_cooltime = crypto.encryptToString(QString("%1").arg(cooltime,0,'f',1));

    double chgdelaytime = datamap->value(QString("%1").arg(mb_chgtime))->value.toDouble()/100.0;
    QString S_chgdelaytime = crypto.encryptToString(QString("%1").arg(chgdelaytime,0,'f',1));


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
                      ",injspd_7 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injspd_8 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injspd_9 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injspd_10 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injprs_1 = '"+S_injPressure[0]+"'"
                      ",injprs_2 = '"+S_injPressure[1]+"'"
                      ",injprs_3 = '"+S_injPressure[2]+"'"
                      ",injprs_4 = '"+S_injPressure[3]+"'"
                      ",injprs_5 = '"+S_injPressure[4]+"'"
                      ",injprs_6 = '"+S_injPressure[5]+"'"
                      ",injprs_7 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injprs_8 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injprs_9 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injprs_10 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injpos_1 = '"+S_injPosition[0]+"'"
                      ",injpos_2 = '"+S_injPosition[1]+"'"
                      ",injpos_3 = '"+S_injPosition[2]+"'"
                      ",injpos_4 = '"+S_injPosition[3]+"'"
                      ",injpos_5 = '"+S_injPosition[4]+"'"
                      ",injpos_6 = '"+S_injPosition[5]+"'"
                      ",injpos_7 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injpos_8 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injpos_9 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injpos_10 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdspd_1 = '"+S_hldVel[0]+"'"
                      ",holdspd_2 = '"+S_hldVel[1]+"'"
                      ",holdspd_3 = '"+S_hldVel[2]+"'"
                      ",holdspd_4 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdspd_5 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdprs_1 = '"+S_hldPressure[0]+"'"
                      ",holdprs_2 = '"+S_hldPressure[1]+"'"
                      ",holdprs_3 = '"+S_hldPressure[2]+"'"
                      ",holdprs_4 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdprs_5 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdtime_1 = '"+S_hldTime[0]+"'"
                      ",holdtime_2 = '"+S_hldTime[1]+"'"
                      ",holdtime_3 = '"+S_hldTime[2]+"'"
                      ",holdtime_4 = '"+crypto.encryptToString(QString("0"))+"'"
                      ",holdtime_5 = '"+crypto.encryptToString(QString("0"))+"'"
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
                      ",suckbpos_1 = '"+S_suckbackPosition0+"'"
                      ",suckbpos_2 = '"+S_suckbackPosition2+"'"
                      ",sovpos = '"+crypto.encryptToString(QString("0"))+"'"
                      ",sovprs = '"+S_pr_EX_Holdp+"'"
                      ",sovtime = '"+crypto.encryptToString(QString("0"))+"'"
                      ",injtime = '"+S_injtime+"'"
                      ",injdeltime = '"+S_injdelaytime+"'"
                      ",cooltime = '"+S_cooltime+"'"
                      ",chgdeltime = '"+S_chgdelaytime+"' "
                );
    update_temp.append(QString("where machine_name = '%1'").arg(mancine_name));
    mysqlquery1.exec(update_temp);


}

void es600_base_locgic::TB_REC_save(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);

    current_shotcount = datamap->value(QString("%1").arg(mb_production_count))->value.toInt();
    if(before_shotcount<0){
        before_shotcount = current_shotcount;
        //modbus_write_register(ctx,mb_actstatus,0);
    }
    if(before_shotcount!=current_shotcount){
        before_shotcount=current_shotcount;
//    int actstatus = datamap->value(QString("%1").arg(mb_actstatus))->value.toInt();
    int fooldata =datamap->value(QString("%1").arg(mb_SHOTDATA_fooldata))->value.toInt();
    double fillingtime = datamap->value(QString("%1").arg(mb_SHOTDATA_fillingtime))->value.toDouble()/100.0;
    double plasticizing_time = datamap->value(QString("%1").arg(mb_SHOTDATA_plasticizing_time))->value.toDouble()/100.0;
    double cycle_time = datamap->value(QString("%1").arg(mb_SHOTDATA_cycle_time))->value.toDouble()/100.0;
    double cushion_position = datamap->value(QString("%1").arg(mb_SHOTDATA_cushion_position))->value.toDouble()/10.0;
    double plasticizing_position = datamap->value(QString("%1").arg(mb_SHOTDATA_plasticizing_position))->value.toDouble()/10.0;
    double temp1=datamap->value(QString("%1").arg(mb_SHOTDATA_temp1))->value.toDouble()/10.0;
    double temp2=datamap->value(QString("%1").arg(mb_SHOTDATA_temp2))->value.toDouble()/10.0;
    double temp3=datamap->value(QString("%1").arg(mb_SHOTDATA_temp3))->value.toDouble()/10.0;
    double temp4=datamap->value(QString("%1").arg(mb_SHOTDATA_temp4))->value.toDouble()/10.0;
    double temp5=datamap->value(QString("%1").arg(mb_SHOTDATA_temp5))->value.toDouble()/10.0;
    double temp6=datamap->value(QString("%1").arg(mb_SHOTDATA_temp6))->value.toDouble()/10.0;
    double temp7=datamap->value(QString("%1").arg(mb_SHOTDATA_temp7))->value.toDouble()/10.0;
    double oil_temp=datamap->value(QString("%1").arg(mb_SHOTDATA_oil_temp))->value.toDouble()/10.0;
    double moldtemp1 = datamap->value(QString("%1").arg(mb_temp9_real))->value.toDouble()/10.0;
    double moldtemp2 = datamap->value(QString("%1").arg(mb_temp10_real))->value.toDouble()/10.0;
    double moldtemp3 = datamap->value(QString("%1").arg(mb_temp11_real))->value.toDouble()/10.0;
    double moldtemp4 = datamap->value(QString("%1").arg(mb_temp12_real))->value.toDouble()/10.0;
    double moldtemp5 = datamap->value(QString("%1").arg(mb_temp13_real))->value.toDouble()/10.0;
    double moldtemp6 = datamap->value(QString("%1").arg(mb_temp14_real))->value.toDouble()/10.0;
    double moldtemp7 = datamap->value(QString("%1").arg(mb_temp15_real))->value.toDouble()/10.0;
    double moldtemp8 = datamap->value(QString("%1").arg(mb_temp16_real))->value.toDouble()/10.0;


    QString moldname = get_moldname();
    double tempues_act[7];
    for(int i=0;i<7;i++){
        tempues_act[i] = datamap->value(QString("%1").arg(mb_tempues1+i*2))->value.toDouble()/10.0;
    }
    if(!tempues_act[0]){
        temp1 = 0.0;
    }
    if(!tempues_act[1]){
        temp2 = 0.0;
    }
    if(!tempues_act[2]){
        temp3 = 0.0;
    }
    if(!tempues_act[3]){
        temp4 = 0.0;
    }
    if(!tempues_act[4]){
        temp5 = 0.0;
    }
    if(!tempues_act[5]){
        temp6 = 0.0;
    }
    if(!tempues_act[6]){
        temp7 = 0.0;
    }

    int moldtempuse_act[8];
    for(int i=0;i<8;i++){
        moldtempuse_act[i] = datamap->value(QString("%1").arg(mb_moldtempuse1+i*2))->value.toInt();
    }
    if(!moldtempuse_act[0]){
        moldtemp1 = 0.0;
    }
    if(!moldtempuse_act[1]){
        moldtemp2 = 0.0;
    }
    if(!moldtempuse_act[2]){
        moldtemp3 = 0.0;
    }
    if(!moldtempuse_act[3]){
        moldtemp4 = 0.0;
    }
    if(!moldtempuse_act[4]){
        moldtemp5 = 0.0;
    }
    if(!moldtempuse_act[5]){
        moldtemp6 = 0.0;
    }
    if(!moldtempuse_act[6]){
        moldtemp7 = 0.0;
    }
    if(!moldtempuse_act[7]){
        moldtemp8 = 0.0;
    }


//    actstatus = datamap->value(QString("%1").arg(mb_actstatus))->value.toInt();
//    if(actstatus == 1 && before_shotcount){
//        current_shotcount = datamap->value(QString("%1").arg(mb_SHOTDATA_count))->value.toInt();
//    }
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
                                      +QString("%1").arg(fillingtime)+","
                                      +QString("%1").arg(fillingtime)+","
                                      +QString("%1").arg(plasticizing_time)+","
                                      +QString("%1").arg(cycle_time)+","
                                      +"0.0"+","
                                      +QString("%1").arg(cushion_position)+","
                                      +"0.0"+","
                                      +QString("%1").arg(plasticizing_position)+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
                                      +"0.0"+","
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


//        qDebug()<<mysqlquery1.lastQuery();
//        qDebug()<<mysqlquery1.lastError().text();

        QString Inj_Velocitystr;
        int injstep = datamap->value(QString("%1").arg(mb_injstep))->value.toInt();
        double injVelocity[6];
        for(int i=0;i<6;i++){
            injVelocity[i] = datamap->value(QString("%1").arg(mb_injVelocity1+i*2))->value.toDouble()/100.0;
        }
        if(injstep>0){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[0]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
            injVelocity[0] = 0.0;
        }
        if(injstep>1){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[1]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
            injVelocity[1] = 0.0;
        }
        if(injstep>2){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[2]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
            injVelocity[2] = 0.0;
        }
        if(injstep>3){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[3]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
            injVelocity[3] = 0.0;
        }
        if(injstep>4){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[4]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
            injVelocity[4] = 0.0;
        }
        if(injstep>5){
            Inj_Velocitystr.append(QString("%1").arg(injVelocity[5]));
        }else{
            Inj_Velocitystr.append(QString("%1").arg(0.0));
            injVelocity[5] = 0.0;
        }
        QString injPressurestr;
        double injPressure[6];
        for(int i=0;i<6;i++){
            injPressure[i] = datamap->value(QString("%1").arg(mb_injPressure1+i*2))->value.toDouble();
        }
        if(injstep>0){
            injPressurestr.append(QString("%1/").arg(injPressure[0]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
            injPressure[0] = 0.0;
        }
        if(injstep>1){
            injPressurestr.append(QString("%1/").arg(injPressure[1]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
            injPressure[1] = 0.0;
        }
        if(injstep>2){
            injPressurestr.append(QString("%1/").arg(injPressure[2]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
            injPressure[2] = 0.0;
        }
        if(injstep>3){
            injPressurestr.append(QString("%1/").arg(injPressure[3]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
            injPressure[3] = 0.0;
        }
        if(injstep>4){
            injPressurestr.append(QString("%1/").arg(injPressure[4]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
            injPressure[4] = 0.0;
        }
        if(injstep>5){
            injPressurestr.append(QString("%1").arg(injPressure[5]));
        }else{
            injPressurestr.append(QString("%1").arg(0.0));
            injPressure[5] = 0.0;
        }
        QString injPositionstr;
        double injPosition[6];
        for(int i=0;i<6;i++){
            injPosition[i] = datamap->value(QString("%1").arg(mb_injPosition1+i*2))->value.toDouble()/10.0;
        }
        if(injstep>0){
            injPositionstr.append(QString("%1/").arg(injPosition[0]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
            injPosition[0] = 0.0;
        }
        if(injstep>1){
            injPositionstr.append(QString("%1/").arg(injPosition[1]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
            injPosition[1] = 0.0;
        }
        if(injstep>2){
            injPositionstr.append(QString("%1/").arg(injPosition[2]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
            injPosition[2] = 0.0;
        }
        if(injstep>3){
            injPositionstr.append(QString("%1/").arg(injPosition[3]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
            injPosition[3] = 0.0;
        }
        if(injstep>4){
            injPositionstr.append(QString("%1/").arg(injPosition[4]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
            injPosition[4] = 0.0;
        }
        if(injstep>5){
            injPositionstr.append(QString("%1").arg(injPosition[5]));
        }else{
            injPositionstr.append(QString("%1").arg(0.0));
            injPosition[5] = 0.0;
        }

        int hldstep = datamap->value(QString("%1").arg(mb_hldstep))->value.toInt();
        QString hldPressurestr;
        double hldPressure[3];
        for(int i=0;i<3;i++){
            hldPressure[i] = datamap->value(QString("%1").arg(mb_hldPressure1+i*2))->value.toDouble();
        }
        if(hldstep>0){
            hldPressurestr.append(QString("%1/").arg(hldPressure[0]));
        }else{
            hldPressurestr.append(QString("%1/").arg(0.0));
            hldPressure[0] = 0.0;
        }
        if(hldstep>1){
            hldPressurestr.append(QString("%1/").arg(hldPressure[1]));
        }else{
            hldPressurestr.append(QString("%1/").arg(0.0));
            hldPressure[1] = 0.0;
        }
        if(hldstep>2){
            hldPressurestr.append(QString("%1").arg(hldPressure[2]));
        }else{
            hldPressurestr.append(QString("%1").arg(0.0));
            hldPressure[2] = 0.0;
        }

        QString hldTimestr;
        double hldTime[3];
        for(int i=0;i<3;i++){
            hldTime[i] = datamap->value(QString("%1").arg(mb_hldTime1+i*2))->value.toDouble();
        }
        if(hldstep>0){
            hldTimestr.append(QString("%1/").arg(hldTime[0]));
        }else{
            hldTimestr.append(QString("%1/").arg(0.0));
            hldTime[0] = 0.0;
        }
        if(hldstep>1){
            hldTimestr.append(QString("%1/").arg(hldTime[1]));
        }else{
            hldTimestr.append(QString("%1/").arg(0.0));
            hldTime[1] = 0.0;
        }
        if(hldstep>2){
            hldTimestr.append(QString("%1").arg(hldTime[2]));
        }else{
            hldTimestr.append(QString("%1").arg(0.0));
            hldTime[2] = 0.0;
        }

        QString hldVelstr;
        double hldVel[3];
        for(int i=0;i<3;i++){
            hldVel[i] = datamap->value(QString("%1").arg(mb_hldVel1+i*2))->value.toDouble()/100.0;
        }
        if(hldstep>0){
            hldVelstr.append(QString("%1/").arg(hldVel[0]));
        }else{
            hldVelstr.append(QString("%1/").arg(0.0));
            hldVel[0] = 0.0;
        }
        if(hldstep>1){
            hldVelstr.append(QString("%1/").arg(hldVel[1]));
        }else{
            hldVelstr.append(QString("%1/").arg(0.0));
            hldVel[1] = 0.0;
        }
        if(hldstep>2){
            hldVelstr.append(QString("%1").arg(hldVel[2]));
        }else{
            hldVelstr.append(QString("%1").arg(0.0));
            hldVel[2] = 0.0;
        }

        QString chgPositionstr;
        double chgPosition[4];
        for(int i=0;i<4;i++){
            chgPosition[i] = datamap->value(QString("%1").arg(mb_chgPosition1+i*2))->value.toDouble()/10.0;
        }
        chgPositionstr = QString("%1/%2/%3/%4").arg(chgPosition[0]).arg(chgPosition[1])
                                               .arg(chgPosition[2]).arg(chgPosition[3]);
        QString chgSpeedstr;
        double chgSpeed[4];
        for(int i=0;i<4;i++){
            chgSpeed[i] = datamap->value(QString("%1").arg(mb_chgSpeed1+i*2))->value.toDouble()/100.0;
        }
        chgSpeedstr = QString("%1/%2/%3/%4").arg(chgSpeed[0]).arg(chgSpeed[1])
                                               .arg(chgSpeed[2]).arg(chgSpeed[3]);
        QString backPressurestr;
        double backPressure[4];
        for(int i=0;i<4;i++){
            backPressure[i] = datamap->value(QString("%1").arg(mb_backPressure1+i*2))->value.toDouble();
        }
        backPressurestr = QString("%1/%2/%3/%4").arg(backPressure[0]).arg(backPressure[1])
                                               .arg(backPressure[2]).arg(backPressure[3]);

        QString suckbackPositionstr;
        double suckbackPosition2 = datamap->value(QString("%1").arg(mb_suckbackPosition2))->value.toDouble()/10.0;;
        suckbackPositionstr = QString("%1/%2").arg(0.0).arg(suckbackPosition2);

        QString suckbackSpeedstr;
        double suckbackSpeed1 = datamap->value(QString("%1").arg(mb_suckbackSpeed1))->value.toDouble()/100.0;;
        double suckbackSpeed2 = datamap->value(QString("%1").arg(mb_suckbackSpeed2))->value.toDouble()/100.0;;
        suckbackSpeedstr = QString("%1/%2").arg(suckbackSpeed1).arg(suckbackSpeed2);

        double tempues[7];
        for(int i=0;i<7;i++){
            tempues[i] = datamap->value(QString("%1").arg(mb_tempues1+i*2))->value.toDouble()/10.0;
        }

        QString tempsetstr;
        double tempset[7];
        for(int i=0;i<7;i++){
            tempset[i] = datamap->value(QString("%1").arg(mb_tempset1+i*2))->value.toDouble()/10.0;
        }
        double oilset = datamap->value(QString("%1").arg(mb_oilset))->value.toDouble()/10.0;
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

        tempsetstr.append(QString("%1/").arg(oilset));

        if(tempues[6]){
            tempsetstr.append(QString("%1").arg(tempset[6]));
        }else{
            tempsetstr.append(QString("%1").arg(0.0));
            tempset[6] = 0.0;
        }


        int moldtempuse[8];
        for(int i=0;i<8;i++){
            moldtempuse[i] = datamap->value(QString("%1").arg(mb_moldtempuse1+i*2))->value.toInt();
        }

        QString moldtempstr;
        double moldtempset[8];
        for(int i=0;i<8;i++){
            moldtempset[i] = datamap->value(QString("%1").arg(mb_moldtempset1+i*2))->value.toDouble()/10.0;
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
        double injtime = datamap->value(QString("%1").arg(mb_injtime))->value.toDouble()/10.0;
        double cooltime = datamap->value(QString("%1").arg(mb_cooltime))->value.toDouble()/10.0;
        double chgtime = datamap->value(QString("%1").arg(mb_chgtime))->value.toDouble()/10.0;
        timerstr = QString("%1/%2/%3/%4").arg(injtime).arg(cooltime).arg("0.0").arg(chgtime);

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
                +"0.0"+","
                +"0.0"+","
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
//        qDebug()<<mysqlquery1.lastError().text();

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
                         "("
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
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(injPressure[0],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[1],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[2],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[3],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[4],0,'f',1)+","
                         ""+QString("%1").arg(injPressure[5],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(injPosition[0],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[1],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[2],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[3],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[4],0,'f',1)+","
                         ""+QString("%1").arg(injPosition[5],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[0],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[1],0,'f',1)+","
                         ""+QString("%1").arg(hldPressure[2],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(hldTime[0],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[1],0,'f',1)+","
                         ""+QString("%1").arg(hldTime[2],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(hldVel[0],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[1],0,'f',1)+","
                         ""+QString("%1").arg(hldVel[2],0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(0.0,0,'f',1)+","
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
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(suckbackPosition2,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed1,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed2,0,'f',1)+","
                         ""+QString("%1").arg(tempset[0],0,'f',1)+","
                         ""+QString("%1").arg(tempset[1],0,'f',1)+","
                         ""+QString("%1").arg(tempset[2],0,'f',1)+","
                         ""+QString("%1").arg(tempset[3],0,'f',1)+","
                         ""+QString("%1").arg(tempset[4],0,'f',1)+","
                         ""+QString("%1").arg(tempset[5],0,'f',1)+","
                         ""+QString("%1").arg(oilset,0,'f',1)+","
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
                         ""+QString("%1").arg(0.0,0,'f',1)+","
                         ""+QString("%1").arg(chgtime,0,'f',1)+")"
               );
    }

    if(queryresult){

    }else {
        remotedb.open();
        qDebug()<<"es600 false";
    }




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


void es600_base_locgic::modbusread_ready(){

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
            reply->deleteLater();
            return;
    const QModbusDataUnit unit = reply->result();
    if (reply->error() == QModbusDevice::NoError) {
        int startaddress = unit.startAddress();
        int value = unit.value(0);
        QString startaddress_str = QString("%1").arg(startaddress);
        if(!datamap->contains(startaddress_str)){
            datamap->insert(startaddress_str,new es600value(startaddress_str,QString("%1").arg(value)));
        }else {
            es600value *tempvalue = datamap->value(startaddress_str);
            tempvalue->value = QString("%1").arg(value);
        }
        if(startaddress == addrlist.last()){  //마지막 데이터 가지 받으면 loop 실행
            waitcondition.wakeAll();
        }
    }
    reply->deleteLater();
}

void es600_base_locgic::alram_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString monitertype = parent_item->type->currentText();
    int alramflagdata = datamap->value(QString("%1").arg(mb_warning_flag))->value.toInt();
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery mysqlquery1(remotedb);
    QStringList keylist = alrammap->keys();
    for(int k=0;k<100;k++){
         alrammap->value(QString("%1").arg(k))->currentflag = false;
    }
    for(int j=0;j<alramflagdata;j++){
        int temp_alramnumber = datamap->value(QString("%1").arg(mb_alrammap1+(j*2)))->value.toInt();
        alrammap->value(QString("%1").arg(temp_alramnumber))->currentflag = true;
    }
    for(int i=0;i<keylist.size();i++){
        bool temp_beforeflag = alrammap->value(keylist.at(i))->beforeflag;
        bool temp_currentflag = alrammap->value(keylist.at(i))->currentflag;
        if(temp_beforeflag!=temp_currentflag){ //이전 플래그와 비교해서 다를때만 실행
            alrammap->value(keylist.at(i))->beforeflag = temp_currentflag;
            QString alramnumber = keylist.at(i);
            if(temp_currentflag){//알람 발생 시점
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
                                 ""+alramnumber+", "
                                 "'"+datetime+"', "
                                 "'1999-01-01 00:00:00',"
                                 "1)"
                                 ";"
                            );
            }else {//알람 해제 시점
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
                                 ""+alramnumber+", "
                                 "'1999-01-01 00:00:00', "
                                 "'"+datetime+"',"
                                 "0)"
                                 ";"
                            );
            }
        }
    }
}

void es600_base_locgic::modbusstatue_change(int state){

    if(state == QModbusDevice::ConnectedState){
        slot_statue_update(true);
    }else if(state == QModbusDevice::ConnectingState){
        slot_statue_update(false);
    }else if(state == QModbusDevice::UnconnectedState){
        qctx->connectDevice();
        slot_statue_update(false);
    }
}
QString es600_base_locgic::get_moldname(){
    unsigned short moldname1 = datamap->value(QString("%1").arg(mb_moldname1))->value.toShort();
    unsigned short moldname2 = datamap->value(QString("%1").arg(mb_moldname2))->value.toShort();
    unsigned short moldname3 = datamap->value(QString("%1").arg(mb_moldname3))->value.toShort();
    unsigned short moldname4 = datamap->value(QString("%1").arg(mb_moldname4))->value.toShort();
    unsigned short moldname5 = datamap->value(QString("%1").arg(mb_moldname5))->value.toShort();
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
    QString moldname = mold_name;
    return moldname;
}
