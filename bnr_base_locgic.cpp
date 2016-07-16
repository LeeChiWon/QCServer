#include "bnr_base_locgic.h"
#include "mslotitem.h"
#include <QSqlError>


Bnr_base_locgic::Bnr_base_locgic(QObject *parentmslot,QObject *parent) :
    QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;


}
Bnr_base_locgic::~Bnr_base_locgic(){

}

bool Bnr_base_locgic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,BNRvalue *>;

    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));

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
        qDebug()<<"bnr DB not open";

    }else {
       qDebug()<<"bnr DB open";
    }
    moudle_thread = new bnr_moudle_thread(this);
    moudle_thread->start();

    initflag=true;
    return initflag;
}
void Bnr_base_locgic::loop(){

    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    //qDebug()<<ip;
    requst_read_value(ip,"BNRbase.asp");
    if(parent_item->type->currentText().split("/").at(1).compare("TAC1XX11")==0){
        requst_read_value(ip,"TAC1XX11warning.asp");
    }
}
void Bnr_base_locgic::requst_read_value(QString ip,QString address){
    QString url = QString("http://%1/%2").arg(ip).arg(address);
    requast.setUrl(url);
    manager.get(requast);

}
void Bnr_base_locgic::managerfinished(QNetworkReply *reply){
    QByteArray temp_data;
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    temp_data = reply->readAll();
    QString htmldata = QString(temp_data);
    int startpoint = 0;
    if(temp_data.size()>0){
    while(true){
            int sdivsearch_point = htmldata.indexOf("<div",startpoint);
            if(sdivsearch_point <0){
                break;
            }
            startpoint= sdivsearch_point + 4; // 4의 의미는 <div length
            int edivsearch_point = htmldata.indexOf("</div>",startpoint);
            QString linesplite = htmldata.mid(sdivsearch_point,edivsearch_point-sdivsearch_point);
            int leserarch_point = linesplite.indexOf(">");

            QString linesplitnamevalue =  linesplite.mid(0,leserarch_point).split('=').at(1);
            linesplitnamevalue = linesplitnamevalue.trimmed();
            int vstartpoint = linesplite.indexOf("\"value\">",0);
            int vendpoint = linesplite.indexOf("</p>",vstartpoint+8); // 4의 의미는 \"value\"> 의 length
            QString linesplitvalue = linesplite.mid(vstartpoint+8,vendpoint-(vstartpoint+8));
            BNRvalue *tempbnrdata;
            if(!datamap->contains(linesplitnamevalue)){
                tempbnrdata = new BNRvalue();
                tempbnrdata->name = linesplitnamevalue;
                datamap->insert(linesplitnamevalue,tempbnrdata);
            } else {
                tempbnrdata = datamap->value(linesplitnamevalue);
            }
            tempbnrdata->value = linesplitvalue;
        }
        if(reply->url().toString().indexOf("BNRbase.asp")>0){
                waitcondition.wakeAll();
        }else if(reply->url().toString().indexOf("TAC1XX11warning.asp")>0){

        }
        if(parent_item->connectlabel->text().indexOf("play-button")<0){
           parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
           parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
        }

    }else {
            parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
            parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
    //qDebug()<<temp_data;
    delete reply;
}

void Bnr_base_locgic::pageloadfinish(bool result){


}

void Bnr_base_locgic::url_bnrbaseloop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);
    QString typetext = parent_item->type->currentText().split('/').at(1);

    if(typetext.indexOf("TA")>=0){
        TA_current_update();
    }else if(typetext.indexOf("TE")>=0){

    }
}

void Bnr_base_locgic::TA_current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);
    SimpleCrypt crypto(CRYPTO_NUMBER);


    QString update_temp = QString("UPDATE temp_table SET ");
    for(int i=1;i<=21;i++){
        QString temp_append;
        if(i == 7){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Oil.ST"))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(0)))
                    .arg(crypto.encryptToString(QString("%1").arg(0)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.System.ATOil"))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(1)));

        }else if(i == 21){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6' ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value)));

        }else {
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value)));
        }
        update_temp.append(temp_append);
    }
    QString temp_append1 = QString("WHERE  machine_name='%1'").arg(mancine_name);
    update_temp.append(temp_append1);

    mysqlquery1.exec(update_temp);


    double object_count = datamap->value("udTotalProd_setpcs")->value.toDouble();
    double production_count = datamap->value("udTotalProd_actpcs")->value.toDouble();
    double achievemen_rate ;
    if(object_count == 0){
        achievemen_rate = 0;
    }else {
        achievemen_rate = (production_count/object_count)*100.0;
    }
    int cycle_time = datamap->value("ACT_DATA.System.AtCycleTime")->value.toInt()/100;
    QTime time;
    time.setHMS(0,0,0);
    QTime cycletime;
    cycletime = time.addSecs(cycle_time);

    int mode = datamap->value("MMI_DATA.Mode")->value.toInt();
    QString modestr;
    if(mode == 1){
        modestr=tr("auto");
    }else if(mode ==2){
        modestr=tr("helfauto");
    }else if(mode ==4){
        modestr=tr("munual");
    }else if(mode ==5){
        modestr=tr("mold");
    }


    update_temp = QString("UPDATE Systeminfo SET production_count = '%1',"
                          "object_count = '%2',"
                          "cabity = '%3',"
                          "achievemen_rate = '%4',"
                          "cycle_time = \'%5\',"
                          "run_mode = \'%6\',"
                          "warning_flag = '%7' "
                          "where machine_name = \'%8\'")
            .arg(crypto.encryptToString(datamap->value("udTotalProd_actpcs")->value))
            .arg(crypto.encryptToString(datamap->value("udTotalProd_setpcs")->value))
            .arg(crypto.encryptToString(datamap->value("uiNoOfCavity")->value))
            .arg(crypto.encryptToString(QString("%1").arg(1,0,'f',achievemen_rate)))
            .arg(cycletime.toString("hh:mm:ss"))
            .arg(crypto.encryptToString(modestr))
            .arg(crypto.encryptToString(datamap->value("MMI_DATA.Alarm.Req")->value))
            .arg(mancine_name);

    bool result = mysqlquery1.exec(update_temp);

    if(result){

    }else {
        qDebug()<<"bnr sql false";
        remotedb.open();
    }

    update_temp=QString("update Recipe_Info set injstep='%1', holdstep='%2', ")
            .arg(crypto.encryptToString(datamap->value("REC_DATA.IP.NrInjectionProfile")->value),
                 crypto.encryptToString(datamap->value("REC_DATA.IP.NrHoldonProfile")->value));

    QStringList Injection[3],HoldPrs[3],Charge[3];
    QStringList ValueNames,SQLValues;
    ValueNames<<"REC_DATA.IP.NEG.Sv"<<"REC_DATA.IP.NEG.Sp"<<"REC_DATA.IP.NEG.Ss"<<"REC_DATA.IP.NEG.SvHoldPrs"<<"REC_DATA.IP.NEG.SpHP"<<"REC_DATA.IP.NEG.St"
             <<"REC_DATA.IP.PL.Sv"<<"REC_DATA.IP.PL.SpBP"<<"REC_DATA.IP.PL.Ss";
    SQLValues<<"injspd"<<"injprs"<<"injpos"<<"holdspd"<<"holdprs"<<"holdtime"<<"chgspd"<<"chgbps"<<"chgpos";
    QString MapValue,SqlValue;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<10; j++)
        {
            //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i)).arg(j))->value;
            //SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i)).arg(j+1).arg(MapValue);
            float temp_injspd = 0;
            float temp_inj = 0;
            QString temp_str =SQLValues.at(i);
            //유압식 사출속도 퍼센트 사용
            if((datamap->value("MA_FIX.OPT.SbInjSpeedPercent")->value.compare("1") == 0)
                    &&(temp_str.compare("injspd") == 0 )){
                float SbInjSpeedPercent = datamap->value("IP_FIX.NEG.SvMaxVis")->value.toFloat()/10.0;
                float temp_injspd2 = datamap->value(QString("%1[%2]").arg(ValueNames.at(i)).arg(j))->value.toFloat()/10.0;
                temp_injspd= temp_injspd2/SbInjSpeedPercent;
                temp_injspd = temp_injspd*100.0;
                MapValue.sprintf("%.1f",temp_injspd);
            }else {
                temp_inj = datamap->value(QString("%1[%2]").arg(ValueNames.at(i)).arg(j))->value.toFloat()/10;
                MapValue.sprintf("%.1f",temp_inj);
            }

            SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i)).arg(j+1).arg(crypto.encryptToString(MapValue));
            Injection[i].append(SqlValue);
            float temp_holdspd = 0.0;
            if(j<5)
            {
                switch(i)
                {
                case HOLDSPD:
                    //유압식 사출속도 퍼센트 사용
                    if(datamap->value("MA_FIX.OPT.SbInjSpeedPercent")->value.compare("1") == 0){
                        float SbInjSpeedPercent = datamap->value("IP_FIX.NEG.SvMaxVis")->value.toFloat()/10.0;
                        float temp_holdspd1 = datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value.toFloat()/10.0;
                        temp_holdspd = temp_holdspd1/SbInjSpeedPercent;
                        temp_holdspd = temp_holdspd*100.0;
                        MapValue.sprintf("%.1f",temp_holdspd);
                    }else{
                        float temp_hold = 0;
                        temp_hold = datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value.toFloat()/10.0;
                        MapValue.sprintf("%.1f",temp_hold);
                    }
                    //MapValue=datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+3)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    HoldPrs[i].append(SqlValue);
                    break;
                case HOLDPRS:
                    MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value.toFloat()/10);
                   // MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+3)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    HoldPrs[i].append(SqlValue);
                    break;
                default:
                    MapValue.sprintf("%.2f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value.toFloat()/100);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+3)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    HoldPrs[i].append(SqlValue);
                    break;
                }
            }

            if(j<3)
            {
                MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value.toFloat()/10);
                //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+6)).arg(j+1).arg(crypto.encryptToString(MapValue));
                Charge[i].append(SqlValue);
            }
        }
    }

    update_temp.append(QString("%1%2%3%4%5%6%7%8%9%10").arg(Injection[INJSPD].at(0),Injection[INJSPD].at(1),Injection[INJSPD].at(2),Injection[INJSPD].at(3),Injection[INJSPD].at(4))
                       .arg(Injection[INJSPD].at(5),Injection[INJSPD].at(6),Injection[INJSPD].at(7),Injection[INJSPD].at(8),Injection[INJSPD].at(9)));

    update_temp.append(QString("%1%2%3%4%5%6%7%8%9%10").arg(Injection[INJPRS].at(0),Injection[INJPRS].at(1),Injection[INJPRS].at(2),Injection[INJPRS].at(3),Injection[INJPRS].at(4))
                       .arg(Injection[INJPRS].at(5),Injection[INJPRS].at(6),Injection[INJPRS].at(7),Injection[INJPRS].at(8),Injection[INJPRS].at(9)));

    update_temp.append(QString("%1%2%3%4%5%6%7%8%9%10").arg(Injection[INJPOS].at(0),Injection[INJPOS].at(1),Injection[INJPOS].at(2),Injection[INJPOS].at(3),Injection[INJPOS].at(4))
                       .arg(Injection[INJPOS].at(5),Injection[INJPOS].at(6),Injection[INJPOS].at(7),Injection[INJPOS].at(8),Injection[INJPOS].at(9)));

    update_temp.append(QString("%1%2%3%4%5").arg(HoldPrs[HOLDSPD].at(0),HoldPrs[HOLDSPD].at(1),HoldPrs[HOLDSPD].at(2),HoldPrs[HOLDSPD].at(3),HoldPrs[HOLDSPD].at(4)));
    update_temp.append(QString("%1%2%3%4%5").arg(HoldPrs[HOLDPRS].at(0),HoldPrs[HOLDPRS].at(1),HoldPrs[HOLDPRS].at(2),HoldPrs[HOLDPRS].at(3),HoldPrs[HOLDPRS].at(4)));
    update_temp.append(QString("%1%2%3%4%5").arg(HoldPrs[HOLDTIME].at(0),HoldPrs[HOLDTIME].at(1),HoldPrs[HOLDTIME].at(2),HoldPrs[HOLDTIME].at(3),HoldPrs[HOLDTIME].at(4)));

    update_temp.append(QString("%1%2%3").arg(Charge[CHGSPD].at(0),Charge[CHGSPD].at(1),Charge[CHGSPD].at(2)));
    update_temp.append(QString("%1%2%3").arg(Charge[CHGBPS].at(0),Charge[CHGBPS].at(1),Charge[CHGBPS].at(2)));
    update_temp.append(QString("%1%2%3").arg(Charge[CHGPOS].at(0),Charge[CHGPOS].at(1),Charge[CHGPOS].at(2)));

    update_temp.append(QString("suckbspd_1='%1', suckbspd_2='%2', suckbpos_1='%3', suckbpos_2='%4',")
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NS.SvDB")->value.toFloat()/10,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NS.SvDA")->value.toFloat()/10,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NS.SsDB")->value.toFloat()/10,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NS.SsDA")->value.toFloat()/10,0,'f',1)))
                       );

    update_temp.append(QString("sovpos='%1', sovprs='%2', injtime='%3', injdeltime='%4', cooltime='%5', chgdeltime='%6' where machine_name='%7'")
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SsSov")->value.toFloat()/10,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpSov")->value.toFloat()/10,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toFloat()/100,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.TIM.StIpDelay")->value.toFloat()/100,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.TIM.StCooling")->value.toFloat()/100,0,'f',1)))
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.TIM.StPlDelay")->value.toFloat()/100,0,'f',1)))
                       .arg(mancine_name));

    mysqlquery1.exec(update_temp);
//    qDebug()<<mysqlquery1.lastQuery();
//     qDebug()<<mysqlquery1.lastError().text();

}

void Bnr_base_locgic::TA_REC_SAVE(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);



}

