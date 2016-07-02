#include "bnr_base_locgic.h"
#include "mslotitem.h"


Bnr_base_locgic::Bnr_base_locgic(QObject *parentmslot,QObject *parent) :
    QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
#if QT_VERSION > QT_VERSION_CHECK(5,6,0)
    basepage = new QWebEnginePage();
    optionpage1 = new QWebEnginePage();
#endif


}
Bnr_base_locgic::~Bnr_base_locgic(){

}

bool Bnr_base_locgic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,BNRvalue *>;
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));
#else
    connect(basepage,SIGNAL(loadFinished(bool)),this,SLOT(pageloadfinish(bool)));
    connect(optionpage1,SIGNAL(loadFinished(bool)),this,SLOT(pageloadfinish(bool)));
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    requast.setUrl(url);
    manager.get(requast);
#else
    if(address.compare("BNRbase.asp") == 0){

        basepage->load(QUrl(url));


    }else if (address.compare("TAC1XX11warning.asp")==0){

        optionpage1->load(QUrl(url));

    }
#endif
}
void Bnr_base_locgic::managerfinished(QNetworkReply *reply){
    QByteArray temp_data;
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    temp_data = reply->readAll();
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    if(temp_data.size()>0){
        webpage.mainFrame()->setHtml(temp_data);
        documents = webpage.mainFrame()->findAllElements("div");
        for(int i=0;i<documents.count();i++){
            document = documents.at(i);
            QString name = document.attribute("name");
            BNRvalue *tempbnrdata;
            if(!datamap->contains(name)){
                tempbnrdata = new BNRvalue();
                tempbnrdata->name = name;
                datamap->insert(name,tempbnrdata);
            } else {
                tempbnrdata = datamap->value(name);
            }
            QString dom  = QString(".value");

            tempbnrdata->value = document.findFirst(dom).toPlainText();
            //qDebug()<<QString("BNRhttp web var = %1, data = %2").arg(tempbnrdata->name).arg(tempbnrdata->value);
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

#endif
}

void Bnr_base_locgic::pageloadfinish(bool result){
#if QT_VERSION > QT_VERSION_CHECK(5,6,0)
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QWebEnginePage *pageView =  (QWebEnginePage *)QObject::sender();
    //       qDebug()<<"tile"<<pageView->title();
    if(result){
        pageView->runJavaScript("var a = document.getElementsByTagName(\"div\");");
        pageView->runJavaScript("a.length",[this,pageView](const QVariant &v){
            pageloadfinish_length = v.toInt();
            for(int i=0;i<pageloadfinish_length;i++){
                QString valuestr = QString("a[%1].getAttribute(\"name\")").arg(i);
                pageView->runJavaScript(valuestr,[this](const QVariant &s){
                    webenginenamestr=s.toString();
                });
                valuestr = QString("a[%1].getElementsByClassName(\"value\")[0].textContent").arg(i);
                pageView->runJavaScript(valuestr,[this,pageView](const QVariant &s){
                    datamap->insert(webenginenamestr,new BNRvalue(webenginenamestr,s.toString()));
                    //qDebug()<<"webenginenamestr = "<<webenginenamestr<<" value = "<<s.toString();
                    if(webenginenamestr.compare("FINISH")==0){ //FINISH로 마지막을 구분한다.
                        if(pageView->url().toString().indexOf("BNRbase.asp")>0){
                            url_bnrbaseloop();
                        }else if(pageView->url().toString().indexOf("TAC1XX11warning.as")>0){

                        }else {

                        }
                    }
                });
            }
        }); //람다 함수의 실행은 소속되어 있는 함수 리턴후 바로 실행 된다.
        if(parent_item->connectlabel->text().indexOf("play-button")<0){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
        }
    }//람다 함수의 실행은 소속되어 있는 함수 리턴후 바로 실행 된다.
    else if(pageView->title().compare("Ststcms")!=0){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
    }
#endif
}

void Bnr_base_locgic::url_bnrbaseloop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();

    QSqlQuery mysqlquery1(remotedb);

    QString update_temp = QString("UPDATE `temp_table` SET ");
    for(int i=1;i<=21;i++){
        QString temp_append ;
        if(i == 7){
            temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6, ")
                    .arg(i)
                    .arg(datamap->value(QString("REC_DATA.HC.Oil.ST"))->value.toDouble()/10)
                    .arg(0)
                    .arg(0)
                    .arg(datamap->value(QString("ACT_DATA.System.ATOil"))->value.toDouble()/10)
                    .arg(1);

        }else if(i == 21){
            temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6 ")
                    .arg(i)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value);

        }else {
            temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6, ")
                    .arg(i)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value.toDouble()/10)
                    .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value);

        }
        update_temp.append(temp_append);
    }
    QString temp_append1 = QString("WHERE  `machine_name`='%1'").arg(mancine_name);
    update_temp.append(temp_append1);

    mysqlquery1.exec(update_temp);

    double object_count = datamap->value("udTotalProd_setpcs")->value.toDouble();
    double production_count = datamap->value("udTotalProd_actpcs")->value.toDouble();
    double achievemen_rate = (production_count/object_count)*100.0;
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


    update_temp = QString("UPDATE Systeminfo SET production_count = %1,"
                          "object_count = %2,"
                          "cabity = %3,"
                          "achievemen_rate = %4,"
                          "cycle_time = \'%5\',"
                          "run_mode = \'%6\',"
                          "warning_flag = %7 "
                          "where machine_name = \'%8\'")
            .arg(datamap->value("udTotalProd_actpcs")->value)
            .arg(datamap->value("udTotalProd_setpcs")->value)
            .arg(datamap->value("uiNoOfCavity")->value)
            .arg(achievemen_rate)
            .arg(cycletime.toString("hh:mm:ss"))
            .arg(modestr)
            .arg(datamap->value("MA_STAT.AbAlarmPending")->value)
            .arg(mancine_name);
    bool result = mysqlquery1.exec(update_temp);
    if(result){

    }else {
        qDebug()<<"bnr sql false";
        remotedb.open();
    }


    update_temp=QString("update Recipe_Info set injstep=%1, holdstep=%2, ")
            .arg(datamap->value("REC_DATA.IP.NrInjectionProfile")->value,datamap->value("REC_DATA.IP.NrHoldonProfile")->value);

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
            MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i)).arg(j))->value.toFloat()/10);
            SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i)).arg(j+1).arg(MapValue);
            Injection[i].append(SqlValue);

            if(j<5)
            {
                switch(i)
                {
                case HOLDSPD:
                    MapValue.sprintf("%.1f",datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value.toFloat()/10);
                    //MapValue=datamap->value(QString("%1").arg(ValueNames.at(i+3)))->value;
                    SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i+3)).arg(j+1).arg(MapValue);
                    HoldPrs[i].append(SqlValue);
                    break;
                case HOLDPRS:
                    MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value.toFloat()/10);
                   // MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value;
                    SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i+3)).arg(j+1).arg(MapValue);
                    HoldPrs[i].append(SqlValue);
                    break;
                default:
                    MapValue.sprintf("%.2f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value.toFloat()/100);
                    //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+3)).arg(j))->value;
                    SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i+3)).arg(j+1).arg(MapValue);
                    HoldPrs[i].append(SqlValue);
                    break;
                }
            }

            if(j<3)
            {
                MapValue.sprintf("%.1f",datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value.toFloat()/10);
                //MapValue=datamap->value(QString("%1[%2]").arg(ValueNames.at(i+6)).arg(j))->value;
                SqlValue=QString("%1_%2=%3, ").arg(SQLValues.at(i+6)).arg(j+1).arg(MapValue);
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

    update_temp.append(QString("suckbspd_1=%1, suckbspd_2=%2, suckbpos_1=%3, suckbpos_2=%4,")
                       .arg(datamap->value("REC_DATA.IP.NS.SvDB")->value.toFloat()/10)
                       .arg(datamap->value("REC_DATA.IP.NS.SvDA")->value.toFloat()/10)
                       .arg(datamap->value("REC_DATA.IP.NS.SsDB")->value.toFloat()/10)
                       .arg(datamap->value("REC_DATA.IP.NS.SsDA")->value.toFloat()/10));

    update_temp.append(QString("sovpos=%1, sovprs=%2, injtime=%3, injdeltime=%4, cooltime=%5, chgdeltime=%6 where machine_name='%7'")
                       .arg(datamap->value("REC_DATA.IP.NEG.SsSov")->value.toFloat()/10)
                       .arg(datamap->value("REC_DATA.IP.NEG.SpSov")->value.toFloat()/10)
                       .arg(datamap->value("REC_DATA.IP.NEG.StSov")->value.toFloat()/100)
                       .arg(datamap->value("REC_DATA.TIM.StIpDelay")->value.toFloat()/100)
                       .arg(datamap->value("REC_DATA.TIM.StCooling")->value.toFloat()/100)
                       .arg(datamap->value("REC_DATA.TIM.StPlDelay")->value.toFloat()/100)
                       .arg(mancine_name));

    mysqlquery1.exec(update_temp);
}

