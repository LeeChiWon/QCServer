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
        qDebug()<<"es600 qctx connect false";
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
    /*
    QByteArray data;
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    while(true){
        data = tcpsocket->readLine();
        if(data.size()==0){
            break;
        }
        QString datastr = QString(data);

        int errormsg = datastr.indexOf("Sorry,");
        if(errormsg>=0){
            tcpsocket->disconnectFromHost();

        }
        int loginfind = datastr.indexOf("login:");

        if(loginfind>0){
            tcpsocket->write("telnet\r\n");
            tcpsocket->write("gefranseven\r\n");
            tcpsocket->flush();
        }
        int startpointfin2 = datastr.indexOf("WorkFile = ");
        if(startpointfin2>=0){
            QString tempfrount = datastr.split(':').at(0);
            QString tempaddrvalue= tempfrount.split('=').at(1);
            tempaddrvalue = tempaddrvalue.trimmed();
            tcpsocket->write(QString("d %1,13\r\n").arg(tempaddrvalue).toLocal8Bit());
            tcpsocket->flush();
        }
        if(datastr.split('*').size()==3){
            QString tempfrontstr = datastr.split('*').at(0);
            QString tempfrontstr1 =  tempfrontstr.split(':').at(1);
            bool msgendflag = false;
            if(tempfrontstr1.indexOf("00")>2){
                 msgendflag = true;
                 temp_mold_info.append(QString(datastr.split('*').at(1)));
            }else if(tempfrontstr1.indexOf("00")<0){
                msgendflag = false;
                temp_mold_info.append(QString(datastr.split('*').at(1)));
            }else{
                msgendflag = false;
            }

            if(msgendflag){
                result_mold_name = temp_mold_info.replace('.',"").trimmed();
                temp_mold_info = "";
                 qDebug()<<result_mold_name;
            }
        }
//        qDebug()<<datastr;
    }
    */
}


void gefranseven_base_logic::loop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    if(modbuscount == 0){
        if(qctx->state()==QModbusDevice::ConnectedState){
            slot_statue_update(true);
            QModbusReply *reply;
            reply = qctx->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,addrlist.at(modbuscount),2),255);
            connect(reply,SIGNAL(finished()),this,SLOT(modbudread_ready()));
            modbuscount++;
        }
    }

    //requst_read_value(ip,"cgi-bin/SevenCgiLib.out");
    //금형 이름을 받기 위함
    /*
    if(tcpsocket->state() == QTcpSocket::ConnectedState){
        tcpsocket->write("WorkFile\r\n");
        tcpsocket->flush();
    }else if (tcpsocket->state() == QTcpSocket::UnconnectedState){
        tcpsocket->connectToHost(parent_item->ip->text(),23);
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }else if (tcpsocket->state() == QTcpSocket::HostLookupState){

    }else if(tcpsocket->state() == QTcpSocket::ConnectingState){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");

    }
    */
    //qDebug()<<tcpsocket->state();
}

void gefranseven_base_logic::gefranseven_base_loop(){

    current_update();
    REC_save();


    //qDebug()<<"baseloop()";
}


void gefranseven_base_logic::modbudread_ready(){
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
            return;
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
            connect(reply,SIGNAL(finished()),this,SLOT(modbudread_ready()));
            modbuscount++;
        }
    }
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
        }
        if(injstep>1){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[1]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
        }
        if(injstep>2){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[2]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
        }
        if(injstep>3){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[3]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
        }
        if(injstep>4){
            Inj_Velocitystr.append(QString("%1/").arg(injVelocity[4]));
        }else{
            Inj_Velocitystr.append(QString("%1/").arg(0.0));
        }
        if(injstep>5){
            Inj_Velocitystr.append(QString("%1").arg(injVelocity[5]));
        }else{
            Inj_Velocitystr.append(QString("%1").arg(0.0));
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
        }
        if(injstep>1){
            injPressurestr.append(QString("%1/").arg(injPressure[1]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
        }
        if(injstep>2){
            injPressurestr.append(QString("%1/").arg(injPressure[2]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
        }
        if(injstep>3){
            injPressurestr.append(QString("%1/").arg(injPressure[3]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
        }
        if(injstep>4){
            injPressurestr.append(QString("%1/").arg(injPressure[4]));
        }else{
            injPressurestr.append(QString("%1/").arg(0.0));
        }
        if(injstep>5){
            injPressurestr.append(QString("%1").arg(injPressure[5]));
        }else{
            injPressurestr.append(QString("%1").arg(0.0));
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
        }
        if(injstep>1){
            injPositionstr.append(QString("%1/").arg(injPosition[1]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
        }
        if(injstep>2){
            injPositionstr.append(QString("%1/").arg(injPosition[2]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
        }
        if(injstep>3){
            injPositionstr.append(QString("%1/").arg(injPosition[3]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
        }
        if(injstep>4){
            injPositionstr.append(QString("%1/").arg(injPosition[4]));
        }else{
            injPositionstr.append(QString("%1/").arg(0.0));
        }
        if(injstep>5){
            injPositionstr.append(QString("%1").arg(injPosition[5]));
        }else{
            injPositionstr.append(QString("%1").arg(0.0));
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
        }
        if(hldstep>1){
            hldPressurestr.append(QString("%1/").arg(hldPressure[1]));
        }else{
            hldPressurestr.append(QString("%1/").arg(0.0));
        }
        if(hldstep>2){
            hldPressurestr.append(QString("%1").arg(hldPressure[2]));
        }else{
            hldPressurestr.append(QString("%1").arg(0.0));
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
        }
        if(hldstep>1){
            hldTimestr.append(QString("%1/").arg(hldTime[1]));
        }else{
            hldTimestr.append(QString("%1/").arg(0.0));
        }
        if(hldstep>2){
            hldTimestr.append(QString("%1").arg(hldTime[2]));
        }else{
            hldTimestr.append(QString("%1").arg(0.0));
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
        }
        if(hldstep>1){
            hldVelstr.append(QString("%1/").arg(hldVel[1]));
        }else{
            hldVelstr.append(QString("%1/").arg(0.0));
        }
        if(hldstep>2){
            hldVelstr.append(QString("%1").arg(hldVel[2]));
        }else{
            hldVelstr.append(QString("%1").arg(0.0));
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
                set_temperature_4 = 0.0;
            }else {
                set_temperature_4 = set_barrel_temperature_5;
            }
            if(tempuse5 == 0){
                set_temperature_5 = 0.0;
            }else {
                set_temperature_5 = set_barrel_temperature_6;
            }
            if(tempuse6 == 0){
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
        }
        if(moldtempuse[1]){
            moldtempstr.append(QString("%1/").arg(moldtempset[1]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[2]){
            moldtempstr.append(QString("%1/").arg(moldtempset[2]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[3]){
            moldtempstr.append(QString("%1/").arg(moldtempset[3]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[4]){
            moldtempstr.append(QString("%1/").arg(moldtempset[4]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[5]){
            moldtempstr.append(QString("%1/").arg(moldtempset[5]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[6]){
            moldtempstr.append(QString("%1/").arg(moldtempset[6]));
        }else{
            moldtempstr.append(QString("%1/").arg(0.0));
        }
        if(moldtempuse[7]){
            moldtempstr.append(QString("%1").arg(moldtempset[7]));
        }else{
            moldtempstr.append(QString("%1").arg(0.0));
        }

        QString timerstr;
        double injdelaytime = datamap->value(QString("%1").arg(gmb_SPR2TM))->value.toDouble()/10.0;
        double cooltime = datamap->value(QString("%1").arg(gmb_COOLTM))->value.toDouble()/10.0;
        double chgdelaytime = datamap->value(QString("%1").arg(gmb_CHDLTM))->value.toDouble()/10.0;
        double injtime = datamap->value(QString("%1").arg(gmb_INJETM))->value.toDouble()/10.0;
        timerstr = QString("%1/%2/%3/%4").arg(injtime).arg(cooltime).arg(injdelaytime).arg(chgdelaytime);

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

    }
}
void gefranseven_base_logic::current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);
    SimpleCrypt crypto(CRYPTO_NUMBER);
    QString update_temp;
    int production_count = datamap->value(QString("%1").arg(gmb_TOTPS))->value.toInt();
    QString mold_name = get_mold_name();
    int object_count = datamap->value(QString("%1").arg(gmb_TOTPR))->value.toInt();
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
                          "warning_data = '%9' "
                          "where machine_name = \'%10\'")
                          .arg(crypto.encryptToString(QString("%1").arg(production_count)))
                          .arg(crypto.encryptToString(mold_name))
                          .arg(crypto.encryptToString(QString("%1").arg(object_count)))
                          .arg(crypto.encryptToString(QString("%1").arg(cabity)))
                          .arg(crypto.encryptToString(QString("%1").arg(achievemen_rate)))
                          .arg(crypto.encryptToString(QString("%1").arg(cycle_time)))
                          .arg(crypto.encryptToString(QString("%1").arg(run_mode)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_flag)))
                          .arg(crypto.encryptToString(QString("%1").arg(warning_data)))
                          .arg(mancine_name);
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
    /*
    QString url = QString("http://%1/%2").arg(ip).arg(address);
    requast.setUrl(url);
    manager.get(requast);
    */
}

void gefranseven_base_logic::managerfinished(QNetworkReply *reply){
    /*
    QByteArray temp_data;
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    temp_data = reply->readAll();
    QString htmldata  = QString(temp_data);
    if(temp_data.size()>0){
         if(parent_item->connectlabel->text().indexOf("play-button")<0){
                parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
                parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
         }
    }else {
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
        delete reply;
        return ;
    }
    if(temp_data.indexOf("PLC Realtime monitoring")>0){
        int startpoint = 0;

        while(true){
            int startpointvar = htmldata.indexOf("<i>",startpoint);
            if(startpointvar<0){
                break;
            }
            startpoint = startpointvar + 3; //numer4 is mean <i> length;
            int endpointlabel = htmldata.indexOf("</i>",startpoint);
            QString varname = htmldata.mid(startpoint,endpointlabel-startpoint);
            int startpoint_value = htmldata.indexOf("[ <b>",endpointlabel);
            startpoint_value = startpoint_value + 5;
            int endpoint_value = htmldata.indexOf("</b>  ]",startpoint_value);
            QString value = htmldata.mid(startpoint_value,endpoint_value-startpoint_value);
            gefranvalue *tempgefdata;
            if(!datamap->contains(varname)){
                 tempgefdata = new gefranvalue();
                 tempgefdata->name = varname;
                 datamap->insert(varname,tempgefdata);
             }else {
                  tempgefdata = datamap->value(varname);
             }
             tempgefdata->value = value;
        }
        waitcondition.wakeAll();

    }else if(temp_data.indexOf("Please enter login information")>0){
        QByteArray postData;
        postData.append("user=master&");
        postData.append("pwd=master&");
        postData.append("button_login=Login");
        QString ip = parent_item->ip->text();
        QString seturl = QString("http://%1/cgi-bin/SevenCgiLib.out").arg(ip);
        requast.setUrl(QUrl(seturl));
        requast.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");

        manager.post(requast,postData);
    }else if (temp_data.indexOf("Select listed variables and click")>0){
        QByteArray postData;
        int startpoint = 0;
        while(true){
            int intputstartpoint =  htmldata.indexOf("<input",startpoint);
            if(intputstartpoint<0){
                break;
            }
            startpoint = intputstartpoint + 6;
            int intputendpoint =  htmldata.indexOf("</input>",startpoint);
            QString linesplit = htmldata.mid(intputstartpoint,intputendpoint-intputstartpoint);
            QString selon_name = linesplit.split('\"').at(3);
            if(selon_name.compare("Submit")==0){
                break;
            }
            QString data  = QString("%1=SEL-ON&").arg(selon_name);
            postData.append(data.toLocal8Bit());
        }
        postData.append("button_getvars=submit");
        QString ip = parent_item->ip->text();
        QString seturl = QString("http://%1/cgi-bin/SevenCgiLib.out").arg(ip);
        requast.setUrl(QUrl(seturl));
        requast.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");
        manager.post(requast,postData);
    }
    delete reply;
    */
}

void gefranseven_base_logic::url_gefranbaseloop(){
    /*
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QSqlQuery mysqlquery1(remotedb);
    bool result;
    result = mysqlquery1.exec("UPDATE `temp_table` SET `temp1_down`=6 WHERE  `machine_name`=\'5호\'");
    qDebug()<<"TOTPS = "<<datamap->value("TOTPS")->value;
    qDebug()<<"LSTCYC = "<<datamap->value("LSTCYC")->value;
    if(result){

    }else {
        remotedb.open();
        qDebug()<<"gefran false";
    }
//    qDebug()<<datamap->value(QString("sp_Injec_0"))->value;
*/
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
