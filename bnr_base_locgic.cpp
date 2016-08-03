#include "bnr_base_locgic.h"
#include "mslotitem.h"
#include <QSqlError>
#include <QDateTime>

Bnr_base_locgic::Bnr_base_locgic(QObject *parentmslot,QObject *parent) :
    QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
    before_prod_actpcs = -1;
    current_prod_actpcs = -1;


}
Bnr_base_locgic::~Bnr_base_locgic(){

}

bool Bnr_base_locgic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,BNRvalue *>;
    alrammap = new QMap<QString,alrammap_data *>;
    for(int i=0;i<ALRAM_MAX;i++){
        alrammap->insert(QString("%1").arg(i),new alrammap_data());
    }

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
    connect(this,SIGNAL(sig_requst_read_value(QString,QString)),this,SLOT(requst_read_value(QString,QString)));
    moudle_thread = new bnr_moudle_thread(this);
    moudle_thread->start();

    initflag=true;
    return initflag;
}
void Bnr_base_locgic::loop(){

    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    //qDebug()<<"bnrreadtime start = "<<QDateTime::currentDateTime().toString("hh:mm:ss");

    requst_read_value(ip,"BNRbase.asp");


}
void Bnr_base_locgic::requst_read_value(QString ip,QString address){
    QString url = QString("http://%1/%2").arg(ip).arg(address);
    requast.setUrl(url);
    manager.get(requast);

}
void Bnr_base_locgic::managerfinished(QNetworkReply *reply){
    QByteArray temp_data;
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    temp_data = reply->readAll();
    if(temp_data.indexOf("Error Occured")>0){
        qDebug()<<"page error ";
        delete reply;
        return ;
    }
    QString htmldata = QString(temp_data);
    //qDebug()<<"bnrreadtime end = "<<QDateTime::currentDateTime().toString("hh:mm:ss");
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
                requst_read_value(ip,"BNRwarning.asp");
        }else if(reply->url().toString().indexOf("BNRwarning.asp")>0){
                waitcondition.wakeAll();
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
        TA_REC_SAVE();
    }else if(typetext.indexOf("TE")>=0){
        TE_current_update();
        TE_REC_SAVE();
    }
    //int alram_flag = datamap->value("MMI_DATA.Alarm.Req")->value.toInt();
    if(datamap->contains("MMI_DATA.Alarm.Gen.Alm[0]")){
        alram_update();
    }
}

void Bnr_base_locgic::TA_current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString ip = parent_item->ip->text();

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

        }else if(i<=8){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value)));

        }else if(i>=9){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i))->value)));
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
//    QTime time;
//    time.setHMS(0,0,0);
//    QTime cycletime;
//    cycletime = time.addSecs(cycle_time);

    int mode = datamap->value("MMI_DATA.Mode")->value.toInt();
//    QString modestr;
//    if(mode == 1){
//        modestr=tr("auto");
//    }else if(mode ==2){
//        modestr=tr("helfauto");
//    }else if(mode ==4){
//        modestr=tr("munual");
//    }else if(mode ==5){
//        modestr=tr("mold");
//    }
    QString moldname = get_mold_name();
    update_temp = QString("UPDATE Systeminfo SET production_count = '%1',"
                          "mold_name = '%2',"
                          "object_count = '%3',"
                          "cabity = '%4',"
                          "achievemen_rate = '%5',"
                          "cycle_time = \'%6\',"
                          "run_mode = \'%7\',"
                          "machine_program = '%8' "
                          "where machine_name = \'%9\'")

            .arg(crypto.encryptToString(datamap->value("udTotalProd_actpcs")->value))
            .arg(crypto.encryptToString(moldname))
            .arg(crypto.encryptToString(datamap->value("udTotalProd_setpcs")->value))
            .arg(crypto.encryptToString(datamap->value("uiNoOfCavity")->value))
            .arg(crypto.encryptToString(QString("%1").arg(achievemen_rate,0,'f')))
            .arg(crypto.encryptToString(QString("%1").arg(cycle_time)))
            .arg(crypto.encryptToString(QString("%1").arg(mode)))
            .arg(crypto.encryptToString(datamap->value("ProgVersion")->value))
            .arg(mancine_name);
    //qDebug()<<"programname="<<datamap->value("ProgVersion")->value;

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

            if(j<4)
            {
                if(j==3){
                    MapValue.sprintf("%.1f",0);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+6)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    Charge[i].append(SqlValue);
                }else {
                    MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value.toFloat()/10);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+6)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    Charge[i].append(SqlValue);
                }
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
    current_prod_actpcs = datamap->value("udTotalProd_actpcs")->value.toInt();
    if(before_prod_actpcs == -1){
        before_prod_actpcs = current_prod_actpcs;
    }else if(before_prod_actpcs != current_prod_actpcs){
        before_prod_actpcs=current_prod_actpcs;

        QString moldname = get_mold_name();

        //qDebug()<<"mold_name = "<<mold_name;
        QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
        QString current_time = QTime::currentTime().toString("hh:mm:ss");
        double temp[21];
        double temp_act[21];
        int tempsbon[21];
       for(int i=0;i<=20;i++){
            int tempdata_sbon = datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i))->value.toInt();
            if( i == 6 ){  //6번은 오일이라서 항상 켜줌
                tempdata_sbon = 1;
            }
            if(tempdata_sbon){
                tempsbon[i] = 1;
                if(i==6){ //6번은 오일이라서 변수가 다름
                    temp[i] = datamap->value(QString("REC_DATA.HC.Oil.ST"))->value.toDouble()/10.0;
                    temp_act[i] = datamap->value(QString("ACT_DATA.System.ATOil"))->value.toDouble()/10.0;
                }else {
                    temp[i] = datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i))->value.toDouble()/10.0;
                    temp_act[i] = datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i))->value.toDouble()/10.0;
                }
            }else {
                tempsbon[i] = 0;
                temp[i] = 0.0;
                temp_act[i] = 0.0;
            }
        }
        mysqlquery1.exec(QString("INSERT INTO shot_data"
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
                                 ",Mold_Temperature_8"
                                 ",Mold_Temperature_9"
                                 ",Mold_Temperature_10"
                                 ",Mold_Temperature_11"
                                 ",Mold_Temperature_12)"
                           "VALUES"
                                 "('"+mancine_name+"',"
                                 "'"+moldname+"',"
                                 "' ',"
                                 "'"+current_date+" "+current_time+"',"
                                 ""+QString("%1").arg(current_prod_actpcs)+","
                                 ""+QString("%1").arg(datamap->value("MA_STAT.AbProductGoodBadCheck")->value.toInt())+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[0]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[1]")->value.toDouble()/1000.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[2]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[3]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[4]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[5]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[6]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[7]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[8]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[9]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[10]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[11]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[12]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[13]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[14]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[15]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[0],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[1],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[2],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[3],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[4],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[5],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[6],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[7],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[9],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[10],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[11],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[12],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[13],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[14],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[15],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[16],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[17],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[18],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[19],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[20],0,'f',1)+")"
                                 )
                         );
//        qDebug()<<mysqlquery1.lastQuery();
//        qDebug()<<mysqlquery1.lastError().text();
        double SbInjSpeedPercent = 0.0;
        if(datamap->value("MA_FIX.OPT.SbInjSpeedPercent")->value.compare("1") == 0){
            SbInjSpeedPercent = datamap->value("IP_FIX.NEG.SvMaxVis")->value.toDouble()/10.0;
        }else {
            SbInjSpeedPercent = 1.0;
        }

        QString Inj_Velocity =QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[0]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[1]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[2]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[3]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[4]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[5]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[6]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[7]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[8]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.Sv[9]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                ;

        QString Inj_Pressure = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[4]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[5]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[6]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[7]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[8]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[9]")->value.toDouble()/10.0,0,'f',1)
                                ;

        QString Inj_Position = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[4]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[5]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[6]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[7]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[8]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[9]")->value.toDouble()/10.0,0,'f',1)
                                ;

        QString SOV_Time = QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/100.0,0,'f',1);

        QString SOV_Position = QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SsSov")->value.toDouble()/10.0,0,'f',1);



        QString Hld_Pressure = QString("%1/%2/%3/%4/%5")
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[4]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString Hld_Time = QString("%1/%2/%3/%4/%5")
                                .arg(datamap->value("REC_DATA.IP.NEG.St[0]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[1]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[2]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[3]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[4]")->value.toDouble()/100.0,0,'f',1)
                                ;

        QString Hld_Vel =QString("%1/%2/%3/%4/%5")
                                .arg(((datamap->value("REC_DATA.IP.NEG.St[0]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.St[1]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.St[2]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.St[3]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.NEG.St[4]")->value.toDouble()/10.0)/SbInjSpeedPercent)*100.0,0,'f',1)
                                ;

        QString Chg_Position = QString("%1/%2/%3")
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[2]")->value.toDouble()/10.0,0,'f',1)
                                ;

        double chgspeedpercent = datamap->value("IP_FIX.PL.SvMaxVis")->value.toDouble()/10.0;

        QString Chg_Speed = QString("%1/%2/%3")
                                .arg(((datamap->value("REC_DATA.IP.PL.Sv[0]")->value.toDouble()/10.0)/chgspeedpercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.PL.Sv[1]")->value.toDouble()/10.0)/chgspeedpercent)*100.0,0,'f',1)
                                .arg(((datamap->value("REC_DATA.IP.PL.Sv[2]")->value.toDouble()/10.0)/chgspeedpercent)*100.0,0,'f',1)
                                ;

        QString BackPressure = QString("%1/%2/%3")
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[2]")->value.toDouble()/10.0,0,'f',1)
                                ;

        QString Suckback_Position = QString("%1/%2")
                                    .arg(datamap->value("REC_DATA.IP.NS.SsDB")->value.toDouble()/10.0,0,'f',1)
                                    .arg(datamap->value("REC_DATA.IP.NS.SsDA")->value.toDouble()/10.0,0,'f',1)
                                    ;

        double SuckbackSpeedpercent = datamap->value("IP_FIX.NS.SvMaxVis")->value.toDouble()/10.0;

        QString Suckback_Speed = QString("%1/%2")
                                        .arg(((datamap->value("REC_DATA.IP.NS.SvDB")->value.toDouble()/10.0)/SuckbackSpeedpercent)*100.0,0,'f',1)
                                        .arg(((datamap->value("REC_DATA.IP.NS.SvDA")->value.toDouble()/10.0)/SuckbackSpeedpercent)*100.0,0,'f',1)
                                        ;



        QString Barrel_Temperature = QString("%1/%2/%3/%4/%5/%6/%7/%8")
                                        .arg(temp[0],0,'f',1)
                                        .arg(temp[1],0,'f',1)
                                        .arg(temp[2],0,'f',1)
                                        .arg(temp[3],0,'f',1)
                                        .arg(temp[4],0,'f',1)
                                        .arg(temp[5],0,'f',1)
                                        .arg(temp[6],0,'f',1)
                                        .arg(temp[7],0,'f',1)
                                        ;
        QString Mold_Temperature = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10/%11/%12")
                                        .arg(temp[9],0,'f',1)
                                        .arg(temp[10],0,'f',1)
                                        .arg(temp[11],0,'f',1)
                                        .arg(temp[12],0,'f',1)
                                        .arg(temp[13],0,'f',1)
                                        .arg(temp[14],0,'f',1)
                                        .arg(temp[15],0,'f',1)
                                        .arg(temp[16],0,'f',1)
                                        .arg(temp[17],0,'f',1)
                                        .arg(temp[18],0,'f',1)
                                        .arg(temp[19],0,'f',1)
                                        .arg(temp[20],0,'f',1)
                                        ;
        QString Timer = QString("%1/%2/%3/%4")
                            .arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/100.0)
                            .arg(datamap->value("REC_DATA.TIM.StCooling")->value.toDouble()/100.0)
                            .arg(datamap->value("REC_DATA.TIM.StIpDelay")->value.toDouble()/100.0)
                            .arg(datamap->value("REC_DATA.TIM.StPlDelay")->value.toDouble()/100.0)
                            ;


        mysqlquery1.exec(QString("INSERT INTO shot_data_rec"
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
                                 "('"+mancine_name+"',"
                                 "'"+moldname+"',"
                                 "' ',"
                                 "'"+current_date+" "+current_time+"',"
                                 ""+QString("%1").arg(current_prod_actpcs)+","
                                 "'"+Inj_Velocity+"',"
                                 "'"+Inj_Pressure+"',"
                                 "'"+Inj_Position+"',"
                                 ""+SOV_Time+","
                                 ""+SOV_Position+","
                                 "'"+Hld_Pressure+"',"
                                 "'"+Hld_Time+"',"
                                 "'"+Hld_Vel+"',"
                                 "'"+Chg_Position+"',"
                                 "'"+Chg_Speed+"',"
                                 "'"+BackPressure+"',"
                                 "'"+Suckback_Position+"',"
                                 "'"+Suckback_Speed+"',"
                                 "'"+Barrel_Temperature+"',"
                                 "'"+Mold_Temperature+"',"
                                 "'"+Timer+"')"
                                 )
                         );

    }
}

void Bnr_base_locgic::TE_current_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString ip = parent_item->ip->text();

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

        }else if(i<=8){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value)));

        }else if(i>=9){
            temp_append = QString("temp%1_set='%2', temp%1_up='%3', temp%1_down='%4', temp%1_real='%5', temp%1_onoff = '%6', ")
                    .arg(i)
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i))->value.toDouble()/10,0,'f',1)))
                    .arg(crypto.encryptToString(QString("%1").arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i))->value)));
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
//    QTime time;
//    time.setHMS(0,0,0);
//    QTime cycletime;
//    cycletime = time.addSecs(cycle_time);

    int mode = datamap->value("MMI_DATA.Mode")->value.toInt();
    QString modestr;
//    if(mode == 1){
//        modestr=tr("auto");
//    }else if(mode ==2){
//        modestr=tr("helfauto");
//    }else if(mode ==4){
//        modestr=tr("munual");
//    }else if(mode ==5){
//        modestr=tr("mold");
//    }
    QString moldname = get_mold_name();
    update_temp = QString("UPDATE Systeminfo SET production_count = '%1',"
                          "mold_name = '%2',"
                          "object_count = '%3',"
                          "cabity = '%4',"
                          "achievemen_rate = '%5',"
                          "cycle_time = \'%6\',"
                          "run_mode = \'%7\',"
                          "machine_program = '%8' "
                          "where machine_name = \'%9\'")

            .arg(crypto.encryptToString(datamap->value("udTotalProd_actpcs")->value))
            .arg(crypto.encryptToString(moldname))
            .arg(crypto.encryptToString(datamap->value("udTotalProd_setpcs")->value))
            .arg(crypto.encryptToString(datamap->value("uiNoOfCavity")->value))
            .arg(crypto.encryptToString(QString("%1").arg(achievemen_rate,0,'f')))
            .arg(crypto.encryptToString(QString("%1").arg(cycle_time)))
            .arg(crypto.encryptToString(QString("%1").arg(mode)))
            .arg(crypto.encryptToString(datamap->value("ProgVersion")->value))
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
    ValueNames<<"REC_DATA.IP.NEG.Sv"<<"REC_DATA.IP.NEG.Sp"<<"REC_DATA.IP.NEG.Ss"<<"IP_FIX.NEG.SvFixHP"<<"REC_DATA.IP.NEG.SpHP"<<"REC_DATA.IP.NEG.St"
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

            temp_inj = datamap->value(QString("%1[%2]").arg(ValueNames.at(i)).arg(j))->value.toFloat()/10;
            MapValue.sprintf("%.1f",temp_inj);

            SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i)).arg(j+1).arg(crypto.encryptToString(MapValue));
            Injection[i].append(SqlValue);
            float temp_holdspd = 0.0;
            float temp_hold = 0;
            if(j<5)
            {
                switch(i)
                {
                case HOLDSPD:

                    temp_hold = datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value.toFloat()/10.0;
                    MapValue.sprintf("%.1f",temp_hold);
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

            if(j<4)
            {
                if(j==3){
                    MapValue.sprintf("%.1f",0);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+6)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    Charge[i].append(SqlValue);
                }else {
                    MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value.toFloat()/10);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                    SqlValue=QString("%1_%2='%3', ").arg(SQLValues.at(i+6)).arg(j+1).arg(crypto.encryptToString(MapValue));
                    Charge[i].append(SqlValue);
                }
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

    update_temp.append(QString("%1%2%3%4").arg(Charge[CHGSPD].at(0),Charge[CHGSPD].at(1),Charge[CHGSPD].at(2),Charge[CHGSPD].at(3)));
    update_temp.append(QString("%1%2%3%4").arg(Charge[CHGBPS].at(0),Charge[CHGBPS].at(1),Charge[CHGBPS].at(2),Charge[CHGBPS].at(3)));
    update_temp.append(QString("%1%2%3%4").arg(Charge[CHGPOS].at(0),Charge[CHGPOS].at(1),Charge[CHGPOS].at(2),Charge[CHGPOS].at(3)));

    update_temp.append(QString("sovtime = '%1',")
                       .arg(crypto.encryptToString(QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toFloat()/1000,0,'f',1)))
                       );

    update_temp.append(QString("ITEM_TYPE = '%1',")
                       .arg(crypto.encryptToString(QString("%1").arg(parent_item->type->currentText())))
                       );

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

//   qDebug()<<mysqlquery1.lastQuery();
//   qDebug()<<mysqlquery1.lastError().text();

}

void Bnr_base_locgic::TE_REC_SAVE(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);
    current_prod_actpcs = datamap->value("udTotalProd_actpcs")->value.toInt();
    if(before_prod_actpcs == -1){
        before_prod_actpcs = current_prod_actpcs;
    }else if(before_prod_actpcs != current_prod_actpcs){
        before_prod_actpcs=current_prod_actpcs;

         QString moldname = get_mold_name();


        double temp[21];
        double temp_act[21];
        int tempsbon[21];
       for(int i=0;i<=20;i++){
            int tempdata_sbon = datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i))->value.toInt();
            if(tempdata_sbon){
                tempsbon[i] = 1;
                temp[i] = datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i))->value.toDouble()/10.0;
                temp_act[i] = datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i))->value.toDouble()/10.0;
            }else {
                tempsbon[i] = 0;
                temp[i] = 0.0;
                temp_act[i] = 0.0;
            }
        }
        //qDebug()<<"mold_name = "<<mold_name;
        QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
        QString current_time = QTime::currentTime().toString("hh:mm:ss");


        mysqlquery1.exec(QString("INSERT INTO shot_data"
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
                                 ",Mold_Temperature_8"
                                 ",Mold_Temperature_9"
                                 ",Mold_Temperature_10"
                                 ",Mold_Temperature_11"
                                 ",Mold_Temperature_12)"
                           "VALUES"
                                 "('"+mancine_name+"',"
                                 "'"+moldname+"',"
                                 "' ',"
                                 "'"+current_date+" "+current_time+"',"
                                 ""+QString("%1").arg(current_prod_actpcs)+","
                                 ""+QString("%1").arg(datamap->value("MA_STAT.AbProductGoodBadCheck")->value.toInt())+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[0]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[1]")->value.toDouble()/1000.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[2]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[3]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[4]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[5]")->value.toDouble()/100.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[6]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[7]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[8]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[9]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[10]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[11]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[12]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[13]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[14]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(datamap->value("ACT_DATA.SQC.Data[15]")->value.toDouble()/10.0,0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[0],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[1],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[2],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[3],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[4],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[5],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[6],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[7],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[9],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[10],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[11],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[12],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[13],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[14],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[15],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[16],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[17],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[18],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[19],0,'f',1)+","
                                 ""+QString("%1").arg(temp_act[20],0,'f',1)+")"
                                 )
                         );

        QString Inj_Velocity =QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[4]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[5]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[6]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[7]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[8]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sv[9]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString Inj_Pressure = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[4]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[5]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[6]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[7]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[8]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Sp[9]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString Inj_Position = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10")
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[4]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[5]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[6]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[7]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[8]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.Ss[9]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString SOV_Time = QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/1000.0,0,'f',1);

        QString SOV_Position = QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SsSov")->value.toDouble()/100.0,0,'f',1);

        QString Hld_Pressure = QString("%1/%2/%3/%4/%5")
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[2]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[3]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.SpHP[4]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString Hld_Time = QString("%1/%2/%3/%4/%5")
                                .arg(datamap->value("REC_DATA.IP.NEG.St[0]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[1]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[2]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[3]")->value.toDouble()/100.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.NEG.St[4]")->value.toDouble()/100.0,0,'f',1)
                                ;

        QString Hld_Vel =QString("%1/%2/%3/%4/%5")
                                .arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)
                                ;

        QString Chg_Position = QString("%1/%2/%3")
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Ss[2]")->value.toDouble()/10.0,0,'f',1)
                                ;

        QString Chg_Speed = QString("%1/%2/%3")
                                .arg(datamap->value("REC_DATA.IP.PL.Sv[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Sv[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.Sv[2]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString BackPressure = QString("%1/%2/%3")
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[0]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[1]")->value.toDouble()/10.0,0,'f',1)
                                .arg(datamap->value("REC_DATA.IP.PL.SpBP[2]")->value.toDouble()/10.0,0,'f',1)
                                ;
        QString Suckback_Position = QString("%1/%2")
                                    .arg(datamap->value("REC_DATA.IP.NS.SsDB")->value.toDouble()/10.0,0,'f',1)
                                    .arg(datamap->value("REC_DATA.IP.NS.SsDA")->value.toDouble()/10.0,0,'f',1)
                                    ;
        QString Suckback_Speed = QString("%1/%2")
                                        .arg(datamap->value("REC_DATA.IP.NS.SvDB")->value.toDouble()/10.0,0,'f',1)
                                        .arg(datamap->value("REC_DATA.IP.NS.SvDA")->value.toDouble()/10.0,0,'f',1)
                                        ;

       QString Barrel_Temperature = QString("%1/%2/%3/%4/%5/%6/%7/%8")
                                       .arg(temp[0],0,'f',1)
                                       .arg(temp[1],0,'f',1)
                                       .arg(temp[2],0,'f',1)
                                       .arg(temp[3],0,'f',1)
                                       .arg(temp[4],0,'f',1)
                                       .arg(temp[5],0,'f',1)
                                       .arg(temp[6],0,'f',1)
                                       .arg(temp[7],0,'f',1)
                                       ;
       QString Mold_Temperature = QString("%1/%2/%3/%4/%5/%6/%7/%8/%9/%10/%11/%12")
                                       .arg(temp[9],0,'f',1)
                                       .arg(temp[10],0,'f',1)
                                       .arg(temp[11],0,'f',1)
                                       .arg(temp[12],0,'f',1)
                                       .arg(temp[13],0,'f',1)
                                       .arg(temp[14],0,'f',1)
                                       .arg(temp[15],0,'f',1)
                                       .arg(temp[16],0,'f',1)
                                       .arg(temp[17],0,'f',1)
                                       .arg(temp[18],0,'f',1)
                                       .arg(temp[19],0,'f',1)
                                       .arg(temp[20],0,'f',1)
                                       ;

       QString Timer = QString("%1/%2/%3/%4")
                           .arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/100.0)
                           .arg(datamap->value("REC_DATA.TIM.StCooling")->value.toDouble()/100.0)
                           .arg(datamap->value("REC_DATA.TIM.StIpDelay")->value.toDouble()/100.0)
                           .arg(datamap->value("REC_DATA.TIM.StPlDelay")->value.toDouble()/100.0)
                           ;

       mysqlquery1.exec(QString("INSERT INTO shot_data_rec"
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
                                "('"+mancine_name+"',"
                                "'"+moldname+"',"
                                "' ',"
                                "'"+current_date+" "+current_time+"',"
                                ""+QString("%1").arg(current_prod_actpcs)+","
                                "'"+Inj_Velocity+"',"
                                "'"+Inj_Pressure+"',"
                                "'"+Inj_Position+"',"
                                ""+SOV_Time+","
                                ""+SOV_Position+","
                                "'"+Hld_Pressure+"',"
                                "'"+Hld_Time+"',"
                                "'"+Hld_Vel+"',"
                                "'"+Chg_Position+"',"
                                "'"+Chg_Speed+"',"
                                "'"+BackPressure+"',"
                                "'"+Suckback_Position+"',"
                                "'"+Suckback_Speed+"',"
                                "'"+Barrel_Temperature+"',"
                                "'"+Mold_Temperature+"',"
                                "'"+Timer+"')"
                                )
                        );
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
                        ",Barrel_Temperature_oil"
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
                        "'"+mancine_name+"',"
                        "'"+moldname+"',"
                        "' ',"
                        "'"+current_date+" "+current_time+"',"
                        ""+QString("%1").arg(current_prod_actpcs)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NrInjectionProfile")->value.toInt())+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NrHoldonProfile")->value.toInt())+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[3]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[4]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[5]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[6]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[7]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[8]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sv[9]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[3]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[4]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[5]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[6]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[7]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[8]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Sp[9]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[3]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[4]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[5]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[6]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[7]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[8]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.Ss[9]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/1000.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SsSov")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpHP[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpHP[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpHP[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpHP[3]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.SpHP[4]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.St[0]")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.St[1]")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.St[2]")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.St[3]")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.St[4]")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("IP_FIX.NEG.SvFixHP")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Ss[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Ss[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Ss[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(0.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Sv[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Sv[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.Sv[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(0.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.SpBP[0]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.SpBP[1]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.PL.SpBP[2]")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(0.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NS.SsDB")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NS.SsDA")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NS.SvDB")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NS.SvDA")->value.toDouble()/10.0,0,'f',1)+","
                        ""+QString("%1").arg(temp[0],0,'f',1)+","
                        ""+QString("%1").arg(temp[1],0,'f',1)+","
                        ""+QString("%1").arg(temp[2],0,'f',1)+","
                        ""+QString("%1").arg(temp[3],0,'f',1)+","
                        ""+QString("%1").arg(temp[4],0,'f',1)+","
                        ""+QString("%1").arg(temp[5],0,'f',1)+","
                        ""+QString("%1").arg(temp[6],0,'f',1)+","
                        ""+QString("%1").arg(0.0,0,'f',1)+","
                        ""+QString("%1").arg(temp[7],0,'f',1)+","
                        ""+QString("%1").arg(temp[9],0,'f',1)+","
                        ""+QString("%1").arg(temp[10],0,'f',1)+","
                        ""+QString("%1").arg(temp[11],0,'f',1)+","
                        ""+QString("%1").arg(temp[12],0,'f',1)+","
                        ""+QString("%1").arg(temp[13],0,'f',1)+","
                        ""+QString("%1").arg(temp[14],0,'f',1)+","
                        ""+QString("%1").arg(temp[15],0,'f',1)+","
                        ""+QString("%1").arg(temp[16],0,'f',1)+","
                        ""+QString("%1").arg(temp[17],0,'f',1)+","
                        ""+QString("%1").arg(temp[18],0,'f',1)+","
                        ""+QString("%1").arg(temp[19],0,'f',1)+","
                        ""+QString("%1").arg(temp[20],0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.TIM.StCooling")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.TIM.StIpDelay")->value.toDouble()/100.0,0,'f',1)+","
                        ""+QString("%1").arg(datamap->value("REC_DATA.TIM.StPlDelay")->value.toDouble()/100.0,0,'f',1)+")"
              );
//       qDebug()<<mysqlquery1.lastQuery();
//       qDebug()<<mysqlquery1.lastError().text();
    }

}

void Bnr_base_locgic::alram_update(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QString monitertype = parent_item->type->currentText();

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    SimpleCrypt crypto(CRYPTO_NUMBER);
    QSqlQuery mysqlquery1(remotedb);
    QStringList keylist = alrammap->keys();
    QString warning_data="";

    for(int k=0;k<GEN_ALRAM_MAX-1;k++){
         int result = datamap->value(QString("MMI_DATA.Alarm.Gen.Alm[%1]").arg(k))->value.toInt();
         alrammap->value(QString("%1").arg(k+1))->currentflag = result;
         if(result){
            warning_data.append(QString("%1/").arg(k+1));
         }
    }

    for(int a=0;a<4;a++){
        for(int b=0;b<10;b++){
            int number = (a*10)+b+HtGr_ALRAM_START;
            int result = 0;
//            if(datamap->contains(QString("MMI_DATA.Alarm.HtGr[%1].Alm[%1]").arg(a).arg(b))){
               result = datamap->value(QString("MMI_DATA.Alarm.HtGr[%1].Alm[%2]").arg(a).arg(b))->value.toInt();
//            }

           alrammap->value(QString("%1").arg(number))->currentflag = result;
           if(result){
               warning_data.append(QString("%1/").arg(number));
           }
        }
    }

    for(int a=0;a<33;a++){
        for(int b=0;b<10;b++){
            int number = (a*10)+b+Htzo_ALRAM_START;
            int result = 0;
//            if(datamap->contains(QString("MMI_DATA.Alarm.HtZo[%1].Alm[%1]").arg(a).arg(b))){
                result = datamap->value(QString("MMI_DATA.Alarm.HtZo[%1].Alm[%2]").arg(a).arg(b))->value.toInt();
//            }
           alrammap->value(QString("%1").arg(number))->currentflag = result;
           if(result){
               warning_data.append(QString("%1/").arg(number));
           }
        }
    }

    for(int a=1;a<21;a++){
        for(int b=0;b<10;b++){
            int number = (a*10)+b+Ax_ALRAM_START;
            int result =0;
            result = datamap->value(QString("MMI_DATA.Alarm.HtZo[%1].Alm[%2]").arg(a).arg(b))->value.toInt();
           alrammap->value(QString("%1").arg(number))->currentflag = result;
           if(result){
               warning_data.append(QString("%1/").arg(number));
           }
        }
    }

    if(datamap->value("MMI_DATA.Alarm.Req")->value.toInt()==1){
        mysqlquery1.exec(QString("UPDATE Systeminfo SET warning_flag = '%1', "
                                 "warning_data = '%2' "
                                 "where machine_name = '%3'"
                                 ).arg(crypto.encryptToString(QString("1")))
                                  .arg(crypto.encryptToString(QString(warning_data)))
                                  .arg(mancine_name)
                         );

    }else {
        mysqlquery1.exec(QString("UPDATE Systeminfo SET warning_flag = '%1', "
                              "warning_data = '%2' "
                              "where machine_name = '%3'"
                              ).arg(crypto.encryptToString(QString("0")))
                               .arg(crypto.encryptToString(QString(" ")))
                               .arg(mancine_name)
                         );
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
                                 "Alarm_End_Time) "
                                 "VALUES "
                                 "('"+mancine_name+"',"
                                 "'"+monitertype+"', "
                                 ""+alramnumber+", "
                                 "'"+datetime+"', "
                                 "'1999-01-01 00:00:00')"
                                 ";"
                            );
            }else {//알람 해제 시점
                mysqlquery1.exec("INSERT INTO "
                                 "Alarm_Log (Machine_Name,"
                                 "Controller_Info,"
                                 "Alarm_Number,"
                                 "Alarm_Start_Time,"
                                 "Alarm_End_Time) "
                                 "VALUES "
                                 "('"+mancine_name+"',"
                                 "'"+monitertype+"', "
                                 ""+alramnumber+", "
                                 "'1999-01-01 00:00:00', "
                                 "'"+datetime+"')"
                                 ";"
                            );
            }
        }
    }
}
QString Bnr_base_locgic::get_mold_name(){
    QByteArray mold_name;
    for(int i=0;i<31;i++){
        unsigned int temp_value = datamap->value(QString("ModbusDspRecipeName[%1]").arg(i))->value.toUInt();
        if(temp_value == 0){
            break;
        }
        mold_name.append(temp_value);
    }
    QString moldname = QString(mold_name);
    return moldname;
}
