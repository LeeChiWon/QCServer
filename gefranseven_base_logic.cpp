#include "gefranseven_base_logic.h"
#include "mslotitem.h"

gefranseven_base_logic::gefranseven_base_logic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
    modbuscount=0;
    before_shotcount =-1;
    current_shotcount =-1;
}

bool gefranseven_base_logic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,gefranvalue *>;
    alrammap = new QMap<QString,alrammap_data *> ;
    //connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));
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
        qDebug()<<"gefran DB not open";

    }else {
       qDebug()<<"gefran DB open";
    }
    QSqlQuery remotequery(remotedb);
    remotequery.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(parent_item->machinenametext));
    if(remotequery.next()){
        QString optionvalue = remotequery.value("option_data").toString();
    }

    /*
    tcpsocket = new QTcpSocket();
    tcpsocket->connectToHost(parent_item->ip->text(),23);
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(telnetreadready()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(telnetjoinerror(QAbstractSocket::SocketError)));
    */
    modbuscount = 0;
    qctx = new QModbusTcpClient(this);
    qctx->setConnectionParameter(QModbusDevice::NetworkAddressParameter,parent_item->iptext);
    qctx->setConnectionParameter(QModbusDevice::NetworkPortParameter,502);
    qctx->setTimeout(3000);

    if(!qctx->connectDevice()){
        qDebug()<<"gefran qctx connect false";
    }else {

    }


    connect(qctx,&QModbusClient::stateChanged,this,&gefranseven_base_logic::modbusstatue_change);

    addrlist.append(gmb_temp1_down);
    addrlist.append(gmb_temp2_down);
    addrlist.append(gmb_temp3_down);
    addrlist.append(gmb_temp4_down);
    addrlist.append(gmb_temp5_down);
    addrlist.append(gmb_temp6_down);
    addrlist.append(gmb_temp7_down);

    addrlist.append(gmb_temp1_up);
    addrlist.append(gmb_temp2_up);
    addrlist.append(gmb_temp3_up);
    addrlist.append(gmb_temp4_up);
    addrlist.append(gmb_temp5_up);
    addrlist.append(gmb_temp6_up);
    addrlist.append(gmb_temp7_up);

    addrlist.append(gmb_tempuse1);
    addrlist.append(gmb_tempuse2);
    addrlist.append(gmb_tempuse3);
    addrlist.append(gmb_tempuse4);
    addrlist.append(gmb_tempuse5);
    addrlist.append(gmb_tempuse6);
    addrlist.append(gmb_tempuse7);

    addrlist.append(gmb_oiluse);
    addrlist.append(gmb_oil_real);
    addrlist.append(gmb_oil_down);
    addrlist.append(gmb_oil_up);

    addrlist.append(gmb_temp1_real);
    addrlist.append(gmb_temp2_real);
    addrlist.append(gmb_temp3_real);
    addrlist.append(gmb_temp4_real);
    addrlist.append(gmb_temp5_real);
    addrlist.append(gmb_temp6_real);
    addrlist.append(gmb_temp7_real);

    addrlist.append(gmb_moldtempset1);
    addrlist.append(gmb_moldtempset2);
    addrlist.append(gmb_moldtempset3);
    addrlist.append(gmb_moldtempset4);
    addrlist.append(gmb_moldtempset5);
    addrlist.append(gmb_moldtempset6);
    addrlist.append(gmb_moldtempset7);
    addrlist.append(gmb_moldtempset8);

    addrlist.append(gmb_moldtemp1_down);
    addrlist.append(gmb_moldtemp2_down);
    addrlist.append(gmb_moldtemp3_down);
    addrlist.append(gmb_moldtemp4_down);
    addrlist.append(gmb_moldtemp5_down);
    addrlist.append(gmb_moldtemp6_down);
    addrlist.append(gmb_moldtemp7_down);
    addrlist.append(gmb_moldtemp8_down);

    addrlist.append(gmb_moldtemp1_up);
    addrlist.append(gmb_moldtemp2_up);
    addrlist.append(gmb_moldtemp3_up);
    addrlist.append(gmb_moldtemp4_up);
    addrlist.append(gmb_moldtemp5_up);
    addrlist.append(gmb_moldtemp6_up);
    addrlist.append(gmb_moldtemp7_up);
    addrlist.append(gmb_moldtemp8_up);

    addrlist.append(gmb_moldtempuse1);
    addrlist.append(gmb_moldtempuse2);
    addrlist.append(gmb_moldtempuse3);
    addrlist.append(gmb_moldtempuse4);
    addrlist.append(gmb_moldtempuse5);
    addrlist.append(gmb_moldtempuse6);
    addrlist.append(gmb_moldtempuse7);
    addrlist.append(gmb_moldtempuse8);

    addrlist.append(gmb_moldtemp1_real);
    addrlist.append(gmb_moldtemp2_real);
    addrlist.append(gmb_moldtemp3_real);
    addrlist.append(gmb_moldtemp4_real);
    addrlist.append(gmb_moldtemp5_real);
    addrlist.append(gmb_moldtemp6_real);
    addrlist.append(gmb_moldtemp7_real);
    addrlist.append(gmb_moldtemp8_real);

    addrlist.append(gmb_TOTPERCENT);
    addrlist.append(gmb_TOTPR);
    addrlist.append(gmb_TOTPS);
    addrlist.append(gmb_CYCLCT);
    addrlist.append(gmb_CAVITY);

    addrlist.append(gmb_mANUAL);
    addrlist.append(gmb_lSemiAuto);
    addrlist.append(gmb_lFullAuto);
    addrlist.append(gmb_MADJSL);

    addrlist.append(gmb_dfl_Alarm);
    addrlist.append(gmb_dmt_RunMode);
    addrlist.append(gmb_dfl_Motor);
    addrlist.append(gmb_dfl_Heater);

    addrlist.append(gmb_WorkFile_1);
    addrlist.append(gmb_WorkFile_2);
    addrlist.append(gmb_WorkFile_3);
    addrlist.append(gmb_WorkFile_4);
    addrlist.append(gmb_WorkFile_5);
    addrlist.append(gmb_WorkFile_6);
    addrlist.append(gmb_WorkFile_7);
    addrlist.append(gmb_WorkFile_8);
    addrlist.append(gmb_WorkFile_9);
    addrlist.append(gmb_WorkFile_10);
    addrlist.append(gmb_WorkFile_11);
    addrlist.append(gmb_WorkFile_12);
    addrlist.append(gmb_WorkFile_13);
    addrlist.append(gmb_WorkFile_14);
    addrlist.append(gmb_WorkFile_15);
    addrlist.append(gmb_WorkFile_16);
    addrlist.append(gmb_WorkFile_17);
    addrlist.append(gmb_WorkFile_18);
    addrlist.append(gmb_WorkFile_19);
    addrlist.append(gmb_WorkFile_20);
    addrlist.append(gmb_WorkFile_21);
    addrlist.append(gmb_WorkFile_22);
    addrlist.append(gmb_WorkFile_23);
    addrlist.append(gmb_WorkFile_24);
    addrlist.append(gmb_WorkFile_25);
    addrlist.append(gmb_WorkFile_26);
    addrlist.append(gmb_WorkFile_27);
    addrlist.append(gmb_WorkFile_28);
    addrlist.append(gmb_WorkFile_29);

    addrlist.append(gmb_MCLSTK);
    addrlist.append(gmb_INJETK);
    addrlist.append(gmb_CHRGTK);
    addrlist.append(gmb_CYCLTK);
    addrlist.append(gmb_MCLSTK1);
    addrlist.append(gmb_CUSPOS);
    addrlist.append(gmb_HLDPOS);
    addrlist.append(gmb_CRGPOS);
    addrlist.append(gmb_MDOPOS);
    addrlist.append(gmb_MAXINS);
    addrlist.append(gmb_MAXRPM);
    addrlist.append(gmb_AVRRPM);
    addrlist.append(gmb_MAXINP);
    addrlist.append(gmb_HLDPRS);
    addrlist.append(gmb_MAXBPR);
    addrlist.append(gmb_AVRBPR);

    addrlist.append(gmb_shotdata_injectiontime);
    addrlist.append(gmb_shotdata_ChargeTime);
    addrlist.append(gmb_shotdata_CycleTime);
    addrlist.append(gmb_shotdata_MoldCloseTime);
    addrlist.append(gmb_shotdata_CushionPos);
    addrlist.append(gmb_shotdata_HoldStartPos);
    addrlist.append(gmb_shotdata_ChargeEndPos);
    addrlist.append(gmb_shotdata_MaxInjectSpd);
    addrlist.append(gmb_shotdata_MaxChargeSpd);
    addrlist.append(gmb_shotdata_AvgChargeSpd);
    addrlist.append(gmb_shotdata_MaxInjectPrs);
    addrlist.append(gmb_shotdata_HoldPrs);
    addrlist.append(gmb_shotdata_MaxBackPrs);
    addrlist.append(gmb_shotdata_AvgBackPrs);
    addrlist.append(gmb_shotdata_Temperature1);
    addrlist.append(gmb_shotdata_Temperature2);
    addrlist.append(gmb_shotdata_Temperature3);
    addrlist.append(gmb_shotdata_Temperature4);
    addrlist.append(gmb_shotdata_Temperature5);
    addrlist.append(gmb_shotdata_Temperature6);
    addrlist.append(gmb_shotdata_Temperature7);
    addrlist.append(gmb_shotdata_Temperature8);
    addrlist.append(gmb_shotdata_MoldOpenPos);
    addrlist.append(gmb_shotdata_CycleCounter);

    addrlist.append(gmb_SetInjectionStep);
    addrlist.append(gmb_SetHoldingStep);

    addrlist.append(gmb_sp_Inject_0);
    addrlist.append(gmb_sp_Inject_1);
    addrlist.append(gmb_sp_Inject_2);
    addrlist.append(gmb_sp_Inject_3);
    addrlist.append(gmb_sp_Inject_4);
    addrlist.append(gmb_sp_Inject_5);

    addrlist.append(gmb_pr_Inject_0);
    addrlist.append(gmb_pr_Inject_1);
    addrlist.append(gmb_pr_Inject_2);
    addrlist.append(gmb_pr_Inject_3);
    addrlist.append(gmb_pr_Inject_4);
    addrlist.append(gmb_pr_Inject_5);

    addrlist.append(gmb_po_Inject_0);
    addrlist.append(gmb_po_Inject_1);
    addrlist.append(gmb_po_Inject_2);
    addrlist.append(gmb_po_Inject_3);
    addrlist.append(gmb_po_Inject_4);
    addrlist.append(gmb_po_Inject_5);

    addrlist.append(gmb_sp_Holdp_0);
    addrlist.append(gmb_sp_Holdp_1);
    addrlist.append(gmb_sp_Holdp_2);

    addrlist.append(gmb_pr_Holdp_0);
    addrlist.append(gmb_pr_Holdp_1);
    addrlist.append(gmb_pr_Holdp_2);

    addrlist.append(gmb_ts_Holdp_0);
    addrlist.append(gmb_ts_Holdp_1);
    addrlist.append(gmb_ts_Holdp_2);


    addrlist.append(gmb_sp_Charge_0);
    addrlist.append(gmb_sp_Charge_1);
    addrlist.append(gmb_sp_Charge_2);
    addrlist.append(gmb_sp_Charge_3);

    addrlist.append(gmb_pr_Charge_0);
    addrlist.append(gmb_pr_Charge_1);
    addrlist.append(gmb_pr_Charge_2);
    addrlist.append(gmb_pr_Charge_3);

    addrlist.append(gmb_po_Charge_0);
    addrlist.append(gmb_po_Charge_1);
    addrlist.append(gmb_po_Charge_2);
    addrlist.append(gmb_po_Charge_3);

    addrlist.append(gmb_bp_Charge_0);
    addrlist.append(gmb_bp_Charge_1);
    addrlist.append(gmb_bp_Charge_2);
    addrlist.append(gmb_bp_Charge_3);

    addrlist.append(gmb_sp_Suckb_0);
    addrlist.append(gmb_sp_Suckb_1);

    addrlist.append(gmb_po_Suckb_0);
    addrlist.append(gmb_po_Suckb_2);

    addrlist.append(gmb_SPR2TM);
    addrlist.append(gmb_COOLTM);
    addrlist.append(gmb_CHDLTM);

    addrlist.append(gmb_alramnumber0);
    addrlist.append(gmb_alramnumber1);
    addrlist.append(gmb_alramnumber2);
    addrlist.append(gmb_alramnumber3);
    addrlist.append(gmb_alramnumber4);
    addrlist.append(gmb_alramnumber5);
    addrlist.append(gmb_alramnumber6);
    addrlist.append(gmb_alramnumber7);
    addrlist.append(gmb_alramnumber8);
    addrlist.append(gmb_alramnumber9);

    addrlist.append(gmb_duv_TopLineNG_0);
    addrlist.append(gmb_duv_TopLineNG_1);
    addrlist.append(gmb_duv_TopLineNG_2);
    addrlist.append(gmb_duv_TopLineNG_3);
    addrlist.append(gmb_duv_TopLineNG_4);
    addrlist.append(gmb_duv_TopLineNG_5);

    addrlist.append(gmb_dts_Holdp_0);
    addrlist.append(gmb_dts_Holdp_1);
    addrlist.append(gmb_dts_Holdp_2);

    addrlist.append(gmb_uv_HeatSet_1);
    addrlist.append(gmb_uv_HeatSet_2);
    addrlist.append(gmb_uv_HeatSet_3);
    addrlist.append(gmb_uv_HeatSet_4);
    addrlist.append(gmb_uv_HeatSet_5);
    addrlist.append(gmb_uv_HeatSet_6);
    addrlist.append(gmb_uv_HeatSet_7);

    addrlist.append(gmb_uv_OilSet);

    addrlist.append(gmb_INJETM);

    addrlist.append(gmb_program_name_1);
    addrlist.append(gmb_program_name_2);
    addrlist.append(gmb_program_name_3);
    addrlist.append(gmb_program_name_4);
    addrlist.append(gmb_program_name_5);
    addrlist.append(gmb_program_name_6);
    addrlist.append(gmb_program_name_7);
    addrlist.append(gmb_program_name_8);
    addrlist.append(gmb_program_name_9);
    addrlist.append(gmb_program_name_10);
    addrlist.append(gmb_program_name_11);
    addrlist.append(gmb_program_name_12);
    addrlist.append(gmb_program_name_13);
    addrlist.append(gmb_program_name_14);
    addrlist.append(gmb_program_name_15);
    addrlist.append(gmb_program_name_16);
    addrlist.append(gmb_program_name_17);
    addrlist.append(gmb_program_name_18);
    addrlist.append(gmb_program_name_19);
    addrlist.append(gmb_program_name_20);

    addrlist.append(gmb_pr_EX_Holdp);

    //index 를 찾지 못해 나오는 에러를 방지
    for(int i=0;i<addrlist.size();i++){
        datamap->insert(QString("%1").arg(addrlist.at(i))
                        ,new gefranvalue(QString("%1").arg(addrlist.at(i)),QString("%1").arg(-1)));
    }
    //기본 옵션이 없으면 기본옵션을 update 해줌
    if(parent_item->gefranset_popup == 0){
        parent_item->gefranset_popup = new gefransetup_popup(parent_item->machinenametext);
    }else {

    }
    remotequery.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(parent_item->machinenametext));
    if(remotequery.next()){
        QString option_data = remotequery.value("option_data").toString();
        QStringList option_data_list = option_data.split(',');

        //size가 1이면 기존에 update가 된것이 없기 때문에 기본 으로 해줌.
        if(option_data_list.size()==1){
           parent_item->gefranset_popup->default_option_insert();
        }
    }

    moudle_thread = new gefranseven_moudle_thread(this);
    moudle_thread->start();
    initflag=true;
    return initflag;
}

//금형 이름 받는 로직
void gefranseven_base_logic::telnetreadready(){

}


void gefranseven_base_logic::loop(){
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

void gefranseven_base_logic::gefranseven_base_loop(){

    current_update();
    REC_save();
    alram_update();

}


void gefranseven_base_logic::modbusread_ready(){
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
            datamap->insert(startaddress_str,new gefranvalue(startaddress_str,QString("%1").arg(value)));
        }else {
            gefranvalue *tempvalue = datamap->value(startaddress_str);
            tempvalue->value = QString("%1").arg(value);
        }
        if(startaddress == addrlist.last()){  //마지막 데이터 가지 받으면 loop 실행
            waitcondition.wakeAll();
        }
    }else {
        //qDebug()<<"modbus error = "<<reply->errorString();

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

void gefranseven_base_logic::REC_save(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery remotequery(remotedb);

    //옵션 사용 파싱 및 적용
    bool hoperuse = true;
    remotequery.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(mancine_name));
    if(remotequery.next()){
        QString option_data = remotequery.value("option_data").toString();
        QStringList option_data_list = option_data.split(',');
        //size가 1이면 기존에 update가 된것이 없기 때문에 기본 으로 해줌.
        if(option_data_list.size()==1){

        }else {
            for(int i=0;i<option_data_list.size()-1;i++){
                QString list_data = option_data_list.at(i);
                QString list_valuename = list_data.split('=').at(0);
                QString list_valueresult = list_data.split('=').at(1);
                if(list_valuename.compare("hoperuse")==0){
                    if(list_valueresult.compare("YES") == 0){
                        hoperuse = true;
                    }else{
                        hoperuse = false;
                    }
                }
            }
        }
    }

    current_shotcount = datamap->value(QString("%1").arg(gmb_TOTPR))->value.toInt();
    if(before_shotcount<0){
        before_shotcount = current_shotcount;
        //modbus_write_register(ctx,mb_actstatus,0);
    }
    if(before_shotcount!=current_shotcount){
        before_shotcount=current_shotcount;

        QString mold_name = get_mold_name();

        int fooldata =0;
        int foolflag[6];
        foolflag[0] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_0))->value.toInt();
        foolflag[1] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_1))->value.toInt();
        foolflag[2] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_2))->value.toInt();
        foolflag[3] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_3))->value.toInt();
        foolflag[4] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_4))->value.toInt();
        foolflag[5] = datamap->value(QString("%1").arg(gmb_duv_TopLineNG_5))->value.toInt();
        for(int i=0;i<6;i++){
            if(foolflag[i]==1){
                fooldata = 1;
                break;
            }
        }
        int holdingstep = datamap->value(QString("%1").arg(gmb_SetHoldingStep))->value.toInt();
        int injection_time_temp = datamap->value(QString("%1").arg(gmb_shotdata_injectiontime))->value.toInt();
        if(holdingstep > 0) injection_time_temp += datamap->value(QString("%1").arg(gmb_dts_Holdp_0))->value.toInt();
        if(holdingstep > 1) injection_time_temp += datamap->value(QString("%1").arg(gmb_dts_Holdp_1))->value.toInt();
        if(holdingstep > 2) injection_time_temp += datamap->value(QString("%1").arg(gmb_dts_Holdp_2))->value.toInt();

        double injection_time = (double)injection_time_temp/100.0;
        double filling_time = datamap->value(QString("%1").arg(gmb_shotdata_injectiontime))->value.toDouble()/100.0;
        double plasticizing_time = datamap->value(QString("%1").arg(gmb_shotdata_ChargeTime))->value.toDouble()/100.0;
        double cycle_time = datamap->value(QString("%1").arg(gmb_shotdata_CycleTime))->value.toDouble()/10.0;
        double clamp_close_time = datamap->value(QString("%1").arg(gmb_shotdata_MoldCloseTime))->value.toDouble()/100.0;
        double cushion_position = datamap->value(QString("%1").arg(gmb_shotdata_CushionPos))->value.toDouble()/10.0;
        double switch_over_position = datamap->value(QString("%1").arg(gmb_shotdata_HoldStartPos))->value.toDouble()/10.0;
        double plasticizing_position = datamap->value(QString("%1").arg(gmb_shotdata_ChargeEndPos))->value.toDouble()/10.0;
        double clamp_open_position = datamap->value(QString("%1").arg(gmb_shotdata_MoldOpenPos))->value.toDouble()/10.0;
        double max_injection_speed = datamap->value(QString("%1").arg(gmb_shotdata_MaxInjectSpd))->value.toDouble()/10.0;
        double max_screw_rpm = datamap->value(QString("%1").arg(gmb_shotdata_MaxChargeSpd))->value.toDouble()/10.0;
        double average_screw_rpm = datamap->value(QString("%1").arg(gmb_shotdata_AvgChargeSpd))->value.toDouble()/10.0;
        double max_injection_pressure = datamap->value(QString("%1").arg(gmb_shotdata_MaxInjectPrs))->value.toDouble()/10.0;
        double max_switch_over_pressure = datamap->value(QString("%1").arg(gmb_shotdata_HoldPrs))->value.toDouble()/10.0;
        double max_back_pressure = datamap->value(QString("%1").arg(gmb_shotdata_MaxBackPrs))->value.toDouble()/10.0;
        double average_back_pressure = datamap->value(QString("%1").arg(gmb_shotdata_AvgBackPrs))->value.toDouble()/10.0;
        double barrel_temperature_1 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature1))->value.toDouble()/10.0;
        double barrel_temperature_2 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature2))->value.toDouble()/10.0;
        double barrel_temperature_3 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature3))->value.toDouble()/10.0;
        double barrel_temperature_4 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature4))->value.toDouble()/10.0;
        double barrel_temperature_5 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature5))->value.toDouble()/10.0;
        double barrel_temperature_6 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature6))->value.toDouble()/10.0;
        double barrel_temperature_7 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature7))->value.toDouble()/10.0;
        double barrel_temperature_8 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature8))->value.toDouble()/10.0;
        double oil_temp = barrel_temperature_1;
        double hoper_temp;
        double temperature_1;
        double temperature_2;
        double temperature_3;
        double temperature_4;
        double temperature_5;
        double temperature_6;
        double temperature_7;
        //호퍼 사용시 tempuse1 이 호퍼 tempuse2 가 히터 1
        //호퍼 사용 안할 경우 에는 tempuse1 이 히터1
        int tempuse1 = datamap->value(QString("%1").arg(gmb_tempuse1))->value.toInt();
        int tempuse2 = datamap->value(QString("%1").arg(gmb_tempuse2))->value.toInt();
        int tempuse3 = datamap->value(QString("%1").arg(gmb_tempuse3))->value.toInt();
        int tempuse4 = datamap->value(QString("%1").arg(gmb_tempuse4))->value.toInt();
        int tempuse5 = datamap->value(QString("%1").arg(gmb_tempuse5))->value.toInt();
        int tempuse6 = datamap->value(QString("%1").arg(gmb_tempuse6))->value.toInt();
        int tempuse7 = datamap->value(QString("%1").arg(gmb_tempuse7))->value.toInt();
        int oiluse = datamap->value(QString("%1").arg(gmb_oiluse))->value.toInt();
        if(hoperuse){
            if(tempuse2 == 0){
                temperature_1 = 0.0;
            }else {
                temperature_1 = barrel_temperature_3;
            }
            if(tempuse3 == 0){
                temperature_2 = 0.0;
            }else {
               temperature_2 = barrel_temperature_4;
            }
            if(tempuse4 == 0){
                temperature_3 = 0.0;
            }else {
                temperature_3 = barrel_temperature_5;
            }
            if(tempuse5 == 0){
                temperature_4 = 0.0;
            }else {
                temperature_4 = barrel_temperature_6;
            }
            if(tempuse6  == 0){
                temperature_5 = 0.0;
            }else {
                temperature_5 = barrel_temperature_7;
            }
            if(tempuse7  == 0){
                temperature_6 = 0.0;
            }else {
                temperature_6 = barrel_temperature_8;
            }

            temperature_7 = oil_temp;

            if(tempuse1 == 0){
                hoper_temp = 0.0;
            }else {
                hoper_temp = barrel_temperature_2;
            }
        }else {
            if(tempuse1 == 0){
                temperature_1 = 0.0;
            }else{
                temperature_1 = barrel_temperature_2;
            }
            if(tempuse2 == 0){
                temperature_2 = 0.0;
            }else{
                temperature_2 = barrel_temperature_3;
            }
            if(tempuse3 == 0){
                temperature_3 = 0.0;
            }else{
                temperature_3 = barrel_temperature_4;
            }
            if(tempuse4 == 0){
                temperature_4 = 0.0;
            }else{
                temperature_4 = barrel_temperature_5;
            }
            if(tempuse5 == 0){
                temperature_5 = 0.0;
            }else{
                temperature_5 = barrel_temperature_6;
            }
            if(tempuse6 == 0){
                temperature_6 = 0.0;
            }else{
                temperature_6 = barrel_temperature_7;
            }

            temperature_7 = oil_temp;

            if(tempuse7 == 0){
                hoper_temp = 0.0;
            }else{
                hoper_temp = barrel_temperature_8;
            }
        }
        double mold_temperature_1 = datamap->value(QString("%1").arg(gmb_moldtemp1_real))->value.toDouble()/10.0;
        double mold_temperature_2 = datamap->value(QString("%1").arg(gmb_moldtemp2_real))->value.toDouble()/10.0;
        double mold_temperature_3 = datamap->value(QString("%1").arg(gmb_moldtemp3_real))->value.toDouble()/10.0;
        double mold_temperature_4 = datamap->value(QString("%1").arg(gmb_moldtemp4_real))->value.toDouble()/10.0;
        double mold_temperature_5 = datamap->value(QString("%1").arg(gmb_moldtemp5_real))->value.toDouble()/10.0;
        double mold_temperature_6 = datamap->value(QString("%1").arg(gmb_moldtemp6_real))->value.toDouble()/10.0;
        double mold_temperature_7 = datamap->value(QString("%1").arg(gmb_moldtemp7_real))->value.toDouble()/10.0;
        double mold_temperature_8 = datamap->value(QString("%1").arg(gmb_moldtemp8_real))->value.toDouble()/10.0;
        int moldtempuse_act[8];
        for(int i=0;i<8;i++){
            moldtempuse_act[i] = datamap->value(QString("%1").arg(gmb_moldtempuse1+i*2))->value.toInt();
        }
        if(!moldtempuse_act[0]){
            mold_temperature_1 = 0.0;
        }
        if(!moldtempuse_act[1]){
            mold_temperature_2 = 0.0;
        }
        if(!moldtempuse_act[2]){
            mold_temperature_3 = 0.0;
        }
        if(!moldtempuse_act[3]){
            mold_temperature_4 = 0.0;
        }
        if(!moldtempuse_act[4]){
            mold_temperature_5 = 0.0;
        }
        if(!moldtempuse_act[5]){
            mold_temperature_6 = 0.0;
        }
        if(!moldtempuse_act[6]){
            mold_temperature_7 = 0.0;
        }
        if(!moldtempuse_act[7]){
            mold_temperature_8 = 0.0;
        }


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
                                      +QString("'%1'").arg(mold_name)+","
                                      +"''"+","
                                      +"'"+datetime+"',"
                                      +QString("%1").arg(current_shotcount)+","
                                      +QString("%1").arg(fooldata)+","
                                      +QString("%1").arg(injection_time)+","
                                      +QString("%1").arg(filling_time)+","
                                      +QString("%1").arg(plasticizing_time)+","
                                      +QString("%1").arg(cycle_time)+","
                                      +QString("%1").arg(clamp_close_time)+","
                                      +QString("%1").arg(cushion_position)+","
                                      +QString("%1").arg(switch_over_position)+","
                                      +QString("%1").arg(plasticizing_position)+","
                                      +QString("%1").arg(clamp_open_position)+","
                                      +QString("%1").arg(max_injection_speed)+","
                                      +QString("%1").arg(max_screw_rpm)+","
                                      +QString("%1").arg(average_screw_rpm)+","
                                      +QString("%1").arg(max_injection_pressure)+","
                                      +QString("%1").arg(max_switch_over_pressure)+","
                                      +QString("%1").arg(max_back_pressure)+","
                                      +QString("%1").arg(average_back_pressure)+","
                                      +QString("%1").arg(temperature_1)+","
                                      +QString("%1").arg(temperature_2)+","
                                      +QString("%1").arg(temperature_3)+","
                                      +QString("%1").arg(temperature_4)+","
                                      +QString("%1").arg(temperature_5)+","
                                      +QString("%1").arg(temperature_6)+","
                                      +QString("%1").arg(temperature_7)+","
                                      +QString("%1").arg(hoper_temp)+","
                                      +QString("%1").arg(mold_temperature_1)+","
                                      +QString("%1").arg(mold_temperature_2)+","
                                      +QString("%1").arg(mold_temperature_3)+","
                                      +QString("%1").arg(mold_temperature_4)+","
                                      +QString("%1").arg(mold_temperature_5)+","
                                      +QString("%1").arg(mold_temperature_6)+","
                                      +QString("%1").arg(mold_temperature_7)+","
                                      +QString("%1").arg(mold_temperature_8)+")"
                   );
        queryresult = remotequery.exec(insertquery);
        QString Inj_Velocitystr;
        int injstep = datamap->value(QString("%1").arg(gmb_SetInjectionStep))->value.toInt();
        double injVelocity[6];
        for(int i=0;i<6;i++){
            injVelocity[i] = datamap->value(QString("%1").arg(gmb_sp_Inject_0+i*2))->value.toDouble()/10.0;
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
            injPressure[i] = datamap->value(QString("%1").arg(gmb_pr_Inject_0+i*2))->value.toDouble()/10.0;
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
            injPosition[i] = datamap->value(QString("%1").arg(gmb_po_Inject_0+i*2))->value.toDouble()/10.0;
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
        int hldstep = datamap->value(QString("%1").arg(gmb_SetHoldingStep))->value.toInt();
        QString hldPressurestr;
        double hldPressure[3];
        for(int i=0;i<3;i++){
            hldPressure[i] = datamap->value(QString("%1").arg(gmb_pr_Holdp_0+i*2))->value.toDouble()/10.0;
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
            hldTime[i] = datamap->value(QString("%1").arg(gmb_ts_Holdp_0+i*2))->value.toDouble()/100.0;
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
            hldVel[i] = datamap->value(QString("%1").arg(gmb_sp_Holdp_0+i*2))->value.toDouble()/10.0;
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
            chgPosition[i] = datamap->value(QString("%1").arg(gmb_po_Charge_0+i*2))->value.toDouble()/10.0;
        }
        chgPositionstr = QString("%1/%2/%3/%4").arg(chgPosition[0]).arg(chgPosition[1])
                                               .arg(chgPosition[2]).arg(chgPosition[3]);
        QString chgSpeedstr;
        double chgSpeed[4];
        for(int i=0;i<4;i++){
            chgSpeed[i] = datamap->value(QString("%1").arg(gmb_sp_Charge_0+i*2))->value.toDouble()/10.0;
        }
        chgSpeedstr = QString("%1/%2/%3/%4").arg(chgSpeed[0]).arg(chgSpeed[1])
                                               .arg(chgSpeed[2]).arg(chgSpeed[3]);

        QString backPressurestr;
        double backPressure[4];
        for(int i=0;i<4;i++){
            backPressure[i] = datamap->value(QString("%1").arg(gmb_bp_Charge_0+i*2))->value.toDouble()/10.0;
        }
        backPressurestr = QString("%1/%2/%3/%4").arg(backPressure[0]).arg(backPressure[1])
                                               .arg(backPressure[2]).arg(backPressure[3]);

        QString suckbackPositionstr;
        double suckbackPosition0 = datamap->value(QString("%1").arg(gmb_po_Suckb_0))->value.toDouble()/10.0;;
        double suckbackPosition2 = datamap->value(QString("%1").arg(gmb_po_Suckb_2))->value.toDouble()/10.0;;
        suckbackPositionstr = QString("%1/%2").arg(suckbackPosition0).arg(suckbackPosition2);

        QString suckbackSpeedstr;
        double suckbackSpeed1 = datamap->value(QString("%1").arg(gmb_sp_Suckb_0))->value.toDouble()/10.0;;
        double suckbackSpeed2 = datamap->value(QString("%1").arg(gmb_sp_Suckb_1))->value.toDouble()/10.0;;
        suckbackSpeedstr = QString("%1/%2").arg(suckbackSpeed1).arg(suckbackSpeed2);
        double set_barrel_temperature_1 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_1))->value.toDouble()/10.0;
        double set_barrel_temperature_2 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_2))->value.toDouble()/10.0;
        double set_barrel_temperature_3 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_3))->value.toDouble()/10.0;
        double set_barrel_temperature_4 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_4))->value.toDouble()/10.0;
        double set_barrel_temperature_5 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_5))->value.toDouble()/10.0;
        double set_barrel_temperature_6 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_6))->value.toDouble()/10.0;
        double set_barrel_temperature_7 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_7))->value.toDouble()/10.0;

        double set_temperature_1;
        double set_temperature_2;
        double set_temperature_3;
        double set_temperature_4;
        double set_temperature_5;
        double set_temperature_6;
        double set_oil_temp = datamap->value(QString("%1").arg(gmb_uv_OilSet))->value.toDouble()/10.0;;
        double set_hoper_temp;

        if(hoperuse){
            if(tempuse2 == 0){
                set_temperature_1 = 0.0;
            }else {
                set_temperature_1 = set_barrel_temperature_2;
            }
            if(tempuse3 == 0){
                set_temperature_2 = 0.0;
            }else {
                set_temperature_2 = set_barrel_temperature_3;
            }
            if(tempuse4 == 0){
                set_temperature_3 = 0.0;
            }else {
                set_temperature_3 = set_barrel_temperature_4;
            }
            if(tempuse5 == 0){
                set_temperature_4 = 0.0;
            }else {
                set_temperature_4 = set_barrel_temperature_5;
            }
            if(tempuse6 == 0){
                set_temperature_5 = 0.0;
            }else {
                set_temperature_5 = set_barrel_temperature_6;
            }
            if(tempuse7 == 0){
                set_temperature_6 = 0.0;
            }else {
                set_temperature_6 = set_barrel_temperature_7;
            }
            if(tempuse1 == 0){
                set_hoper_temp = 0.0;
            }else {
                set_hoper_temp = set_barrel_temperature_1;
            }
        }else {
            if(tempuse1 == 0){
                set_temperature_1 = 0.0;
            }else {
                set_temperature_1 = set_barrel_temperature_1;
            }
            if(tempuse2 == 0){
                set_temperature_2 = 0.0;
            }else {
                set_temperature_2 = set_barrel_temperature_2;
            }
            if(tempuse3 == 0){
                set_temperature_3 = 0.0;
            }else {
                set_temperature_3 = set_barrel_temperature_3;
            }
            if(tempuse4 == 0){
                set_temperature_4 = 0.0;
            }else {
                set_temperature_4 = set_barrel_temperature_4;
            }
            if(tempuse5 == 0){
                set_temperature_5 = 0.0;
            }else {
                set_temperature_5 = set_barrel_temperature_5;
            }
            if(tempuse6 == 0){
                set_temperature_6 = 0.0;
            }else {
                set_temperature_6 = set_barrel_temperature_6;
            }
            if(tempuse7 == 0){
                set_hoper_temp = 0.0;
            }else {
                set_hoper_temp = set_barrel_temperature_7;
            }
        }
        QString tempsetstr = QString("%1/%2/%3/%4/%5/%6/%7/%8")
                .arg(set_temperature_1)
                .arg(set_temperature_2)
                .arg(set_temperature_3)
                .arg(set_temperature_4)
                .arg(set_temperature_5)
                .arg(set_temperature_6)
                .arg(set_oil_temp)
                .arg(set_hoper_temp);

        int moldtempuse[8];
        for(int i=0;i<8;i++){
            moldtempuse[i] = datamap->value(QString("%1").arg(gmb_moldtempuse1+i*2))->value.toInt();
        }

        QString moldtempstr;
        double moldtempset[8];
        for(int i=0;i<8;i++){
            moldtempset[i] = datamap->value(QString("%1").arg(gmb_moldtempset1+i*2))->value.toDouble()/10.0;
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
        double injdelaytime = datamap->value(QString("%1").arg(gmb_SPR2TM))->value.toDouble()/100.0;
        double cooltime = datamap->value(QString("%1").arg(gmb_COOLTM))->value.toDouble()/100.0;
        double chgdelaytime = datamap->value(QString("%1").arg(gmb_CHDLTM))->value.toDouble()/100.0;
        double injtime = datamap->value(QString("%1").arg(gmb_INJETM))->value.toDouble()/100.0;
        timerstr = QString("%1/%2/%3/%4").arg(injtime).arg(cooltime).arg(injdelaytime).arg(chgdelaytime);

        double sovprs = datamap->value(QString("%1").arg(gmb_pr_EX_Holdp))->value.toDouble()/10.0;

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
                +QString("'%1'").arg(mold_name)+","
                +"''"+","
                +"'"+datetime+"',"
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

        queryresult = remotequery.exec(insertquery);
        if(queryresult){

        }else {
            remotedb.open();
            qDebug()<<"gefran false";
        }

        remotequery.exec("INSERT INTO shot_data_rec2"
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
                         ""+QString("'%1'").arg(mancine_name)+","
                         ""+QString("'%1'").arg(mold_name)+","
                         "' ',"
                         "'"+datetime+"',"
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
                         ""+QString("%1").arg(sovprs,0,'f',1)+","
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
                         ""+QString("%1").arg(suckbackPosition0,0,'f',1)+","
                         ""+QString("%1").arg(suckbackPosition2,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed1,0,'f',1)+","
                         ""+QString("%1").arg(suckbackSpeed2,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_1,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_2,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_3,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_4,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_5,0,'f',1)+","
                         ""+QString("%1").arg(set_temperature_6,0,'f',1)+","
                         ""+QString("%1").arg(set_oil_temp,0,'f',1)+","
                         ""+QString("%1").arg(set_hoper_temp,0,'f',1)+","
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
                         ""+QString("%1").arg(chgdelaytime,0,'f',1)+")"
               );
    }
}
void gefranseven_base_logic::current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);
    SimpleCrypt crypto(CRYPTO_NUMBER);
    QString update_temp;
    int production_count = datamap->value(QString("%1").arg(gmb_TOTPR))->value.toInt();
    QString mold_name = get_mold_name();
    QString program_name = get_program_name();
    int object_count = datamap->value(QString("%1").arg(gmb_TOTPS))->value.toInt();
    int cabity = datamap->value(QString("%1").arg(gmb_CAVITY))->value.toInt();
    double cycle_time = datamap->value(QString("%1").arg(gmb_CYCLCT))->value.toDouble()/10.0;
    int run_mode =0;
    int mANUAL = datamap->value(QString("%1").arg(gmb_mANUAL))->value.toInt();
    int lSemiAuto = datamap->value(QString("%1").arg(gmb_lSemiAuto))->value.toInt();
    int lFullAuto = datamap->value(QString("%1").arg(gmb_lFullAuto))->value.toInt();
    int MADJSL = datamap->value(QString("%1").arg(gmb_MADJSL))->value.toInt();

    if(mANUAL >0){
        run_mode = 4;
    }else if(lSemiAuto > 0){
        run_mode = 2;
    }else if(lFullAuto > 0){
        run_mode = 1;
    }else if(MADJSL>0){
        run_mode = 5;
    }
    double achievemen_rate = datamap->value(QString("%1").arg(gmb_TOTPERCENT))->value.toDouble()/10.0;
    int warning_flag = datamap->value(QString("%1").arg(gmb_dfl_Alarm))->value.toInt();
    QString warning_data;
    if(warning_flag == 1){
        for(int i=0;i<10;i++){
            short temp_alaram_data = datamap->value(QString("%1").arg(gmb_alramnumber0+(i*2)))->value.toInt();
            if(temp_alaram_data!=-1){
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
                          .arg(crypto.encryptToString(QString("%1").arg(cycle_time)))
                          .arg(crypto.encryptToString(QString("%1").arg(run_mode)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_flag)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_data)))
                          .arg(crypto.encryptToString(QString("%1").arg(program_name)))
                          .arg(mancine_name);
    mysqlquery1.exec(update_temp);

    //옵션 사용 파싱 및 적용
    bool hoperuse = true;
    mysqlquery1.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(mancine_name));
    if(mysqlquery1.next()){
        QString option_data = mysqlquery1.value("option_data").toString();
        QStringList option_data_list = option_data.split(',');
        //size가 1이면 기존에 update가 된것이 없기 때문에 기본 으로 해줌.
        if(option_data_list.size()==1){

        }else {
            for(int i=0;i<option_data_list.size()-1;i++){
                QString list_data = option_data_list.at(i);
                QString list_valuename = list_data.split('=').at(0);
                QString list_valueresult = list_data.split('=').at(1);
                if(list_valuename.compare("hoperuse")==0){
                    if(list_valueresult.compare("YES") == 0){
                        hoperuse = true;
                    }else{
                        hoperuse = false;
                    }
                }
            }
        }
    }

    double barrel_temperature_1 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature1))->value.toDouble()/10.0;
    double barrel_temperature_2 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature2))->value.toDouble()/10.0;
    double barrel_temperature_3 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature3))->value.toDouble()/10.0;
    double barrel_temperature_4 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature4))->value.toDouble()/10.0;
    double barrel_temperature_5 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature5))->value.toDouble()/10.0;
    double barrel_temperature_6 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature6))->value.toDouble()/10.0;
    double barrel_temperature_7 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature7))->value.toDouble()/10.0;
    double barrel_temperature_8 = datamap->value(QString("%1").arg(gmb_shotdata_Temperature8))->value.toDouble()/10.0;
    double oil_temp = barrel_temperature_1;
    double hoper_temp;
    double temperature_1;
    double temperature_2;
    double temperature_3;
    double temperature_4;
    double temperature_5;
    double temperature_6;
    double temperature_7;
    double temperature_real[9];
    int temperatureuse[9];
    //호퍼 사용시 tempuse1 이 호퍼 tempuse2 가 히터 1
    //호퍼 사용 안할 경우 에는 tempuse1 이 히터1
    int tempuse1 = datamap->value(QString("%1").arg(gmb_tempuse1))->value.toInt();
    int tempuse2 = datamap->value(QString("%1").arg(gmb_tempuse2))->value.toInt();
    int tempuse3 = datamap->value(QString("%1").arg(gmb_tempuse3))->value.toInt();
    int tempuse4 = datamap->value(QString("%1").arg(gmb_tempuse4))->value.toInt();
    int tempuse5 = datamap->value(QString("%1").arg(gmb_tempuse5))->value.toInt();
    int tempuse6 = datamap->value(QString("%1").arg(gmb_tempuse6))->value.toInt();
    int tempuse7 = datamap->value(QString("%1").arg(gmb_tempuse7))->value.toInt();
    int oiluse = datamap->value(QString("%1").arg(gmb_oiluse))->value.toInt();
    if(hoperuse){
        if(tempuse2 == 0){
            temperature_1 = 0.0;
            temperature_real[1] = barrel_temperature_3;
            temperatureuse[1] = 0;
        }else {
            temperature_1 = barrel_temperature_3;
            temperature_real[1] = barrel_temperature_3;
            temperatureuse[1] = barrel_temperature_3;
            temperatureuse[1] = 1;
        }
        if(tempuse3 == 0){
            temperature_2 = 0.0;
            temperature_real[2] = barrel_temperature_4;
            temperatureuse[2] = 0;
        }else {
           temperature_2 = barrel_temperature_4;
           temperature_real[2] = barrel_temperature_4;
           temperatureuse[2] = 1;
        }
        if(tempuse4 == 0){
            temperature_3 = 0.0;
            temperature_real[3] = barrel_temperature_5;
            temperatureuse[3] = 0;
        }else {
            temperature_3 = barrel_temperature_5;
            temperature_real[3] = barrel_temperature_5;
            temperatureuse[3] = 0;
        }
        if(tempuse5 == 0){
            temperature_4 = 0.0;
            temperature_real[4] = barrel_temperature_6;
            temperatureuse[4] = 0;
        }else {
            temperature_4 = barrel_temperature_6;
            temperature_real[4] = barrel_temperature_6;
            temperatureuse[4] = 1;
        }
        if(tempuse6  == 0){
            temperature_5 = 0.0;
            temperature_real[5] = barrel_temperature_7;
            temperatureuse[5] = 0;
        }else {
            temperature_5 = barrel_temperature_7;
            temperature_real[5] = barrel_temperature_7;
            temperatureuse[5] = 1;
        }
        if(tempuse7  == 0){
            temperature_6 = 0.0;
            temperature_real[6] = barrel_temperature_8;
            temperatureuse[6] = 0;
        }else {
            temperature_6 = barrel_temperature_8;
            temperature_real[6] = barrel_temperature_8;
            temperatureuse[6] = 1;
        }

        temperature_7 = oil_temp;
        temperature_real[7] = oil_temp;
        temperatureuse[7] = 1;

        if(tempuse1 == 0){
            hoper_temp = 0.0;
            temperature_real[8] = barrel_temperature_2;
            temperatureuse[8] = 0;
        }else {
            hoper_temp = barrel_temperature_2;
            temperature_real[8] = barrel_temperature_2;
            temperatureuse[8] = 1;
        }
    }else {
        if(tempuse1 == 0){
            temperature_1 = 0.0;
            temperature_real[1] = barrel_temperature_2;
            temperatureuse[1] = 0;
        }else{
            temperature_1 = barrel_temperature_2;
            temperature_real[1] = barrel_temperature_2;
            temperatureuse[1] = 1;
        }
        if(tempuse2 == 0){
            temperature_2 = 0.0;
            temperature_real[2] = barrel_temperature_3;
            temperatureuse[2] = 0;
        }else{
            temperature_2 = barrel_temperature_3;
            temperature_real[2]= barrel_temperature_3;
            temperatureuse[2] = 1;
        }
        if(tempuse3 == 0){
            temperature_3 = 0.0;
            temperature_real[3] = barrel_temperature_4;
            temperatureuse[3] = 0;
        }else{
            temperature_3 = barrel_temperature_4;
            temperature_real[3]= barrel_temperature_4;
            temperatureuse[3] = 1;
        }
        if(tempuse4 == 0){
            temperature_4 = 0.0;
            temperature_real[4] = barrel_temperature_5;
            temperatureuse[4] = 0;
        }else{
            temperature_4 = barrel_temperature_5;
            temperature_real[4]= barrel_temperature_5;
            temperatureuse[4] = 1;
        }
        if(tempuse5 == 0){
            temperature_5 = 0.0;
            temperature_real[5] = barrel_temperature_6;
            temperatureuse[5] = 0;
        }else{
            temperature_5 = barrel_temperature_6;
            temperature_real[5] = barrel_temperature_6;
            temperatureuse[5] = 1;
        }
        if(tempuse6 == 0){
            temperature_6 = 0.0;
            temperature_real[6] = barrel_temperature_7;
            temperatureuse[6] = 0;
        }else{
            temperature_6 = barrel_temperature_7;
            temperature_real[6] = barrel_temperature_7;
            temperatureuse[6] = 1;
        }

        temperature_7 = oil_temp;
        temperatureuse[7] = 1;

        if(tempuse7 == 0){
            hoper_temp = 0.0;
            temperature_real[8] = barrel_temperature_8;
            temperatureuse[8] = 0;
        }else{
            hoper_temp = barrel_temperature_8;
            temperature_real[8] = barrel_temperature_8;
            temperatureuse[8] = 1;
        }
    }

    double set_barrel_temperature_1 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_1))->value.toDouble()/10.0;
    double set_barrel_temperature_2 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_2))->value.toDouble()/10.0;
    double set_barrel_temperature_3 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_3))->value.toDouble()/10.0;
    double set_barrel_temperature_4 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_4))->value.toDouble()/10.0;
    double set_barrel_temperature_5 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_5))->value.toDouble()/10.0;
    double set_barrel_temperature_6 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_6))->value.toDouble()/10.0;
    double set_barrel_temperature_7 = datamap->value(QString("%1").arg(gmb_uv_HeatSet_7))->value.toDouble()/10.0;

    double set_temperature_1;
    double set_temperature_2;
    double set_temperature_3;
    double set_temperature_4;
    double set_temperature_5;
    double set_temperature_6;
    double set_oil_temp = datamap->value(QString("%1").arg(gmb_uv_OilSet))->value.toDouble()/10.0;
    double set_hoper_temp;
    double temperature_set[9];
    double temperature_up_set[9];
    double temperature_down_set[9];

    if(hoperuse){
        if(tempuse2 == 0){
            set_temperature_1 = 0.0;
            temperature_set[1] = set_barrel_temperature_2;
        }else {
            set_temperature_1 = set_barrel_temperature_2;
            temperature_set[1] = set_barrel_temperature_2;
        }
        temperature_up_set[1] = datamap->value(QString("%1").arg(gmb_temp2_up))->value.toDouble()/10.0;
        temperature_down_set[1] = datamap->value(QString("%1").arg(gmb_temp2_down))->value.toDouble()/10.0;
        if(tempuse3 == 0){
            set_temperature_2 = 0.0;
            temperature_set[2] = set_barrel_temperature_3;
        }else {
            set_temperature_2 = set_barrel_temperature_3;
            temperature_set[2] = set_barrel_temperature_3;
        }
        temperature_up_set[2] = datamap->value(QString("%1").arg(gmb_temp3_up))->value.toDouble()/10.0;
        temperature_down_set[2] = datamap->value(QString("%1").arg(gmb_temp3_down))->value.toDouble()/10.0;
        if(tempuse4 == 0){
            set_temperature_3 = 0.0;
            temperature_set[3] = set_barrel_temperature_4;
        }else {
            set_temperature_3 = set_barrel_temperature_4;
            temperature_set[3] = set_barrel_temperature_4;
        }
        temperature_up_set[3] = datamap->value(QString("%1").arg(gmb_temp4_up))->value.toDouble()/10.0;
        temperature_down_set[3] = datamap->value(QString("%1").arg(gmb_temp4_down))->value.toDouble()/10.0;
        if(tempuse5 == 0){
            set_temperature_4 = 0.0;
            temperature_set[4] = set_barrel_temperature_5;
        }else {
            set_temperature_4 = set_barrel_temperature_5;
            temperature_set[4] = set_barrel_temperature_5;
        }
        temperature_up_set[4] = datamap->value(QString("%1").arg(gmb_temp5_up))->value.toDouble()/10.0;
        temperature_down_set[4] = datamap->value(QString("%1").arg(gmb_temp5_down))->value.toDouble()/10.0;
        if(tempuse6 == 0){
            set_temperature_5 = 0.0;
            temperature_set[5] = set_barrel_temperature_6;
        }else {
            set_temperature_5 = set_barrel_temperature_6;
            temperature_set[5] = set_barrel_temperature_6;
        }
        temperature_up_set[5] = datamap->value(QString("%1").arg(gmb_temp6_up))->value.toDouble()/10.0;
        temperature_down_set[5] = datamap->value(QString("%1").arg(gmb_temp6_down))->value.toDouble()/10.0;
        if(tempuse7 == 0){
            set_temperature_6 = 0.0;
            temperature_set[6] = set_barrel_temperature_7;
        }else {
            set_temperature_6 = set_barrel_temperature_7;
            temperature_set[6] = set_barrel_temperature_7;
        }
        temperature_up_set[6] = datamap->value(QString("%1").arg(gmb_temp7_up))->value.toDouble()/10.0;
        temperature_down_set[6] = datamap->value(QString("%1").arg(gmb_temp7_down))->value.toDouble()/10.0;

        temperature_set[7] = set_oil_temp;
        temperature_up_set[7] = datamap->value(QString("%1").arg(gmb_oil_up))->value.toDouble()/10.0;
        temperature_down_set[7] = datamap->value(QString("%1").arg(gmb_oil_down))->value.toDouble()/10.0;
        if(tempuse1 == 0){
            set_hoper_temp = 0.0;
            temperature_set[8] = set_barrel_temperature_1;
        }else {
            set_hoper_temp = set_barrel_temperature_1;
            temperature_set[8] = set_barrel_temperature_1;
        }
        temperature_up_set[8] = datamap->value(QString("%1").arg(gmb_temp1_up))->value.toDouble()/10.0;
        temperature_down_set[8] = datamap->value(QString("%1").arg(gmb_temp1_down))->value.toDouble()/10.0;
    }else {
        if(tempuse1 == 0){
            set_temperature_1 = 0.0;
            temperature_set[1] = set_barrel_temperature_1;
        }else {
            set_temperature_1 = set_barrel_temperature_1;
            temperature_set[1] = set_barrel_temperature_1;
        }
        temperature_up_set[1] = datamap->value(QString("%1").arg(gmb_temp1_up))->value.toDouble()/10.0;
        temperature_down_set[1] = datamap->value(QString("%1").arg(gmb_temp1_down))->value.toDouble()/10.0;
        if(tempuse2 == 0){
            set_temperature_2 = 0.0;
            temperature_set[2] = set_barrel_temperature_2;
        }else {
            set_temperature_2 = set_barrel_temperature_2;
            temperature_set[2] = set_barrel_temperature_2;
        }
        temperature_up_set[2] = datamap->value(QString("%1").arg(gmb_temp2_up))->value.toDouble()/10.0;
        temperature_down_set[2] = datamap->value(QString("%1").arg(gmb_temp2_down))->value.toDouble()/10.0;
        if(tempuse3 == 0){
            set_temperature_3 = 0.0;
            temperature_set[3] = set_barrel_temperature_3;
        }else {
            set_temperature_3 = set_barrel_temperature_3;
            temperature_set[3] = set_barrel_temperature_3;
        }
        temperature_up_set[3] = datamap->value(QString("%1").arg(gmb_temp3_up))->value.toDouble()/10.0;
        temperature_down_set[3] = datamap->value(QString("%1").arg(gmb_temp3_down))->value.toDouble()/10.0;
        if(tempuse4 == 0){
            set_temperature_4 = 0.0;
            temperature_set[4] = set_barrel_temperature_4;
        }else {
            set_temperature_4 = set_barrel_temperature_4;
            temperature_set[4] = set_barrel_temperature_4;
        }
        temperature_up_set[4] = datamap->value(QString("%1").arg(gmb_temp4_up))->value.toDouble()/10.0;
        temperature_down_set[4] = datamap->value(QString("%1").arg(gmb_temp4_down))->value.toDouble()/10.0;
        if(tempuse5 == 0){
            set_temperature_5 = 0.0;
            temperature_set[5] = set_barrel_temperature_5;
        }else {
            set_temperature_5 = set_barrel_temperature_5;
            temperature_set[5] = set_barrel_temperature_5;
        }
        temperature_up_set[5] = datamap->value(QString("%1").arg(gmb_temp5_up))->value.toDouble()/10.0;
        temperature_down_set[5] = datamap->value(QString("%1").arg(gmb_temp5_down))->value.toDouble()/10.0;
        if(tempuse6 == 0){
            set_temperature_6 = 0.0;
            temperature_set[6] = set_barrel_temperature_6;
        }else {
            set_temperature_6 = set_barrel_temperature_6;
            temperature_set[6] = set_barrel_temperature_6;
        }
        temperature_up_set[6] = datamap->value(QString("%1").arg(gmb_temp6_up))->value.toDouble()/10.0;
        temperature_down_set[6] = datamap->value(QString("%1").arg(gmb_temp6_down))->value.toDouble()/10.0;

        temperature_set[7] = set_oil_temp;
        temperature_up_set[7] = datamap->value(QString("%1").arg(gmb_oil_up))->value.toDouble()/10.0;
        temperature_down_set[7] = datamap->value(QString("%1").arg(gmb_oil_down))->value.toDouble()/10.0;

        if(tempuse7 == 0){
            set_hoper_temp = 0.0;
            temperature_set[8] = set_barrel_temperature_7;
        }else {
            set_hoper_temp = set_barrel_temperature_7;
            temperature_set[8] = set_barrel_temperature_7;
        }
        temperature_up_set[8] = datamap->value(QString("%1").arg(gmb_temp7_up))->value.toDouble()/10.0;
        temperature_down_set[8] = datamap->value(QString("%1").arg(gmb_temp7_down))->value.toDouble()/10.0;
    }

    int moldtempuse_use[8];
    for(int i=0;i<8;i++){
        moldtempuse_use[i] = datamap->value(QString("%1").arg(gmb_moldtempuse1+i*2))->value.toInt();
    }
    double moldtemp_set[8];
    for(int i=0;i<8;i++){
        moldtemp_set[i] = datamap->value(QString("%1").arg(gmb_moldtempset1+i*2))->value.toDouble()/10.0;
    }
    double moldtempuse_temp[8];
    for(int i=0;i<8;i++){
        moldtempuse_temp[i] = datamap->value(QString("%1").arg(gmb_moldtemp1_real+i*2))->value.toDouble()/10.0;
    }
    double moldtempuse_up_set[8];
    for(int i=0;i<8;i++){
        moldtempuse_up_set[i] = datamap->value(QString("%1").arg(gmb_moldtemp1_up+i*2))->value.toDouble()/10.0;
    }
    double moldtempuse_down_set[8];
    for(int i=0;i<8;i++){
        moldtempuse_down_set[i] = datamap->value(QString("%1").arg(gmb_moldtemp1_down+i*2))->value.toDouble()/10.0;
    }
    update_temp = QString("UPDATE temp_table SET ");
    QString temp_append ;
    for(int i=1;i<=16;i++){
        if(i == 16){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6' ")
                                .arg(i)
                                .arg(crypto.encryptToString(QString("%1").arg(moldtemp_set[i-9],0,'f',1)))
                                .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_up_set[i-9],0,'f',1)))
                                .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_down_set[i-9],0,'f',1)))
                                .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_temp[i-9],0,'f',1)))
                                .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_use[i-9])));
        }else if(i<=8){
         temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                               .arg(i)
                               .arg(crypto.encryptToString(QString("%1").arg(temperature_set[i],0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temperature_up_set[i],0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temperature_down_set[i],0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temperature_real[i],0,'f',1)))
                               .arg(crypto.encryptToString(QString("%1").arg(temperatureuse[i])));

        }else if(i>=9){
         temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                            .arg(i)
                            .arg(crypto.encryptToString(QString("%1").arg(moldtemp_set[i-9],0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_up_set[i-9],0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_down_set[i-9],0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_temp[i-9],0,'f',1)))
                            .arg(crypto.encryptToString(QString("%1").arg(moldtempuse_use[i-9])));

        }
         update_temp.append(temp_append);
    }


    temp_append = QString("WHERE machine_name=\'%1\'").arg(mancine_name);

    update_temp.append(temp_append);

    bool result = mysqlquery1.exec(update_temp);

    QString S_ITEM_TYPE = crypto.encryptToString(parent_item->type->currentText());
    QString S_injstep = crypto.encryptToString(datamap->value(QString("%1").arg(gmb_SetInjectionStep))->value);
    int injstep = datamap->value(QString("%1").arg(gmb_SetInjectionStep))->value.toInt();
    double injVelocity[6];
    QString S_injVelocity[6];
    double injPressure[6];
    QString S_injPressure[6];
    double injPosition[6];
    QString S_injPosition[6];
    for(int i=0;i<6;i++){
        injVelocity[i] = datamap->value(QString("%1").arg(gmb_sp_Inject_0+i*2))->value.toDouble()/10.0;
         injPressure[i] = datamap->value(QString("%1").arg(gmb_pr_Inject_0+i*2))->value.toDouble()/10.0;
         injPosition[i] = datamap->value(QString("%1").arg(gmb_po_Inject_0+i*2))->value.toDouble()/10.0;
        if(injstep<i){
            injVelocity[i] = 0.0;
            injPressure[i] = 0.0;
            injPosition[i] = 0.0;
        }
        S_injVelocity[i] = crypto.encryptToString(QString("%1").arg(injVelocity[i],0,'f',1));
        S_injPressure[i] = crypto.encryptToString(QString("%1").arg(injPressure[i],0,'f',1));
        S_injPosition[i] = crypto.encryptToString(QString("%1").arg(injPosition[i],0,'f',1));
    }
    int hldstep = datamap->value(QString("%1").arg(gmb_SetHoldingStep))->value.toInt();
    QString S_hldstep = crypto.encryptToString(datamap->value(QString("%1").arg(gmb_SetHoldingStep))->value);
    double hldPressure[3];
    QString S_hldPressure[3];
    double hldTime[3];
    QString S_hldTime[3];
    double hldVel[3];
    QString S_hldVel[3];
    for(int i=0;i<3;i++){
        hldPressure[i] = datamap->value(QString("%1").arg(gmb_pr_Holdp_0+i*2))->value.toDouble()/10.0;
        hldTime[i] = datamap->value(QString("%1").arg(gmb_ts_Holdp_0+i*2))->value.toDouble()/10.0;
        hldVel[i] = datamap->value(QString("%1").arg(gmb_sp_Holdp_0+i*2))->value.toDouble()/10.0;
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
        chgPosition[i] = datamap->value(QString("%1").arg(gmb_po_Charge_0+i*2))->value.toDouble()/10.0;
        S_chgPosition[i] = crypto.encryptToString(QString("%1").arg(chgPosition[i],0,'f',1));
        chgSpeed[i] = datamap->value(QString("%1").arg(gmb_sp_Charge_0+i*2))->value.toDouble()/10.0;
        S_chgSpeed[i] = crypto.encryptToString(QString("%1").arg(chgSpeed[i],0,'f',1));
        backPressure[i] = datamap->value(QString("%1").arg(gmb_bp_Charge_0+i*2))->value.toDouble()/10.0;
        S_backPressure[i] = crypto.encryptToString(QString("%1").arg(backPressure[i],0,'f',1));
    }
    double suckbackPosition0 = datamap->value(QString("%1").arg(gmb_po_Suckb_0))->value.toDouble()/10.0;;
    double suckbackPosition2 = datamap->value(QString("%1").arg(gmb_po_Suckb_2))->value.toDouble()/10.0;;
    QString S_suckbackPosition0 = crypto.encryptToString(QString("%1").arg(suckbackPosition0,0,'f',1));
    QString S_suckbackPosition2 = crypto.encryptToString(QString("%1").arg(suckbackPosition2,0,'f',1));
    double suckbackSpeed1 = datamap->value(QString("%1").arg(gmb_sp_Suckb_0))->value.toDouble()/10.0;;
    double suckbackSpeed2 = datamap->value(QString("%1").arg(gmb_sp_Suckb_1))->value.toDouble()/10.0;;
    QString S_suckbackSpeed1  = crypto.encryptToString(QString("%1").arg(suckbackSpeed1,0,'f',1));
    QString S_suckbackSpeed2 = crypto.encryptToString(QString("%1").arg(suckbackSpeed2,0,'f',1));

    double pr_EX_Holdp = datamap->value(QString("%1").arg(gmb_pr_EX_Holdp))->value.toDouble()/10.0;;
    QString S_pr_EX_Holdp = crypto.encryptToString(QString("%1").arg(pr_EX_Holdp,0,'f',1));

    double injtime = datamap->value(QString("%1").arg(gmb_INJETM))->value.toDouble()/100.0;
    QString S_injtime = crypto.encryptToString(QString("%1").arg(injtime,0,'f',1));

    double injdelaytime = datamap->value(QString("%1").arg(gmb_SPR2TM))->value.toDouble()/100.0;
    QString S_injdelaytime = crypto.encryptToString(QString("%1").arg(injdelaytime,0,'f',1));

    double cooltime = datamap->value(QString("%1").arg(gmb_COOLTM))->value.toDouble()/100.0;
    QString S_cooltime = crypto.encryptToString(QString("%1").arg(cooltime,0,'f',1));

    double chgdelaytime = datamap->value(QString("%1").arg(gmb_CHDLTM))->value.toDouble()/100.0;
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


void gefranseven_base_logic::modbusstatue_change(int state){

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

void gefranseven_base_logic::slot_statue_update(bool statue){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    if(statue){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
    }else {
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
}


void gefranseven_base_logic::requst_read_value(QString ip, QString address){

}

void gefranseven_base_logic::managerfinished(QNetworkReply *reply){

}

void gefranseven_base_logic::url_gefranbaseloop(){

}
QString gefranseven_base_logic::get_mold_name(){
    QByteArray mold_name_byte;
    for(int i=0;i<29;i++){
        int temp_mold_name  = datamap->value(QString("%1").arg(gmb_WorkFile_1+i*2))->value.toInt();
        mold_name_byte.append(temp_mold_name);
    }
    QString mold_name = QString(mold_name_byte);
    return mold_name;
}
QString gefranseven_base_logic::get_program_name(){
    QByteArray program_name_byte;
    for(int i=0;i<20;i++){
        int temp_program_name  = datamap->value(QString("%1").arg(gmb_WorkFile_1+i*2))->value.toInt();
        program_name_byte.append(temp_program_name);
    }
    QString program_name = QString(program_name_byte);
    return program_name;
}

void gefranseven_base_logic::alram_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString monitertype = parent_item->type->currentText();
    int alramflagdata = datamap->value(QString("%1").arg(gmb_dfl_Alarm))->value.toInt();
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery mysqlquery1(remotedb);
    QStringList maplist = alrammap->keys();
    for(int j=0;j<maplist.size();j++){
        alrammap_data *tempmapdata;
        tempmapdata = alrammap->value(maplist.at(j));
        tempmapdata->currentflag = false;
    }
    if(alramflagdata > 0){
        for(int i=0;i<10;i++){
            QString temp_alramdata = datamap->value(QString("%1").arg(gmb_alramnumber0+(i*2)))->value;
            alrammap_data *tempmapdata;
            if(temp_alramdata == "65535"){
                continue;
            }
            if(!alrammap->contains(temp_alramdata)){
                tempmapdata = new alrammap_data();
                alrammap->insert(temp_alramdata,tempmapdata);
            }else{
                tempmapdata = alrammap->value(temp_alramdata);
            }
            tempmapdata->currentflag = true;
        }
    }
    maplist = alrammap->keys();
    for(int j=0;j<maplist.size();j++){
        alrammap_data *tempmapdata;
        tempmapdata = alrammap->value(maplist.at(j));
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
                                 ""+maplist.at(j)+", "
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
                                 ""+maplist.at(j)+", "
                                 "'1999-01-01 00:00:00', "
                                 "'"+datetime+"',"
                                 "0)"
                                 ";"
                           );
            }
        }
    }
}
