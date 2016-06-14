#include "bnr_base_locgic.h"
#include "mslotitem.h"

Bnr_base_locgic::Bnr_base_locgic(QObject *parentmslot,QObject *parent) :
    QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
}
bool Bnr_base_locgic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,BNRvalue *>;
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));
#else
    connect(&basepage,SIGNAL(loadFinished(bool)),this,SLOT(pageloadfinish(bool)));
    connect(&optionpage1,SIGNAL(loadFinished(bool)),this,SLOT(pageloadfinish(bool)));
#endif
    remotedb  = QSqlDatabase::database("remotedb");
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
        basepage.load(QUrl(url));
    }else if (address.compare("TAC1XX11warning.asp")==0){
        optionpage1.load(QUrl(url));
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
            url_bnrbaseloop();
        }else if(reply->url().toString().indexOf("TAC1XX11warning.asp")>0){

        }

        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");

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
       if(result){
           QWebEnginePage *page =  (QWebEnginePage *)QObject::sender();
           page->runJavaScript("var a = document.getElementsByTagName(\"div\");");
           page->runJavaScript("a.length",[this,page](const QVariant &v){
                pageloadfinish_length = v.toInt();
                for(int i=0;i<pageloadfinish_length;i++){
                QString valuestr = QString("a[%1].getAttribute(\"name\")").arg(i);
                    page->runJavaScript(valuestr,[this](const QVariant &s){
                         webenginenamestr=s.toString();
                    });
                valuestr = QString("a[%1].getElementsByClassName(\"value\")[0].textContent").arg(i);
                        page->runJavaScript(valuestr,[this,page](const QVariant &s){
                          datamap->insert(webenginenamestr,new BNRvalue(webenginenamestr,s.toString()));
                          //qDebug()<<"webenginenamestr = "<<webenginenamestr<<" value = "<<s.toString();
                          if(webenginenamestr.compare("FINISH")==0){ //FINISH로 마지막을 구분한다.
                               if(page->url().toString().indexOf("BNRbase.asp")>0){
                                    url_bnrbaseloop();
                               }else if(page->url().toString().indexOf("TAC1XX11warning.as")>0){

                               }else {

                               }
                          }
                     });
                }
           }); //람다 함수의 실행은 소속되어 있는 함수 리턴후 바로 실행 된다.
           parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
           parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
       }//람다 함수의 실행은 소속되어 있는 함수 리턴후 바로 실행 된다.
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
                                        .arg(datamap->value(QString("REC_DATA.HC.Oil.ST"))->value)
                                        .arg(0)
                                        .arg(0)
                                        .arg(datamap->value(QString("ACT_DATA.System.ATOil"))->value)
                                        .arg(1);

        }else if(i == 21){
            temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6 ")
                                        .arg(i)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value)
                                        .arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].SbOn").arg(i-1))->value);

        }else {
            temp_append = QString("`temp%1_set`=%2, `temp%1_up`=%3, `temp%1_down`=%4, `temp%1_real`=%5, temp%1_onoff = %6, ")
                                        .arg(i)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].ST").arg(i-1))->value)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STpTol").arg(i-1))->value)
                                        .arg(datamap->value(QString("REC_DATA.HC.Zone[%1].STnTol").arg(i-1))->value)
                                        .arg(datamap->value(QString("ACT_DATA.Zone[%1].AT").arg(i-1))->value)
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
    mysqlquery1.exec(update_temp);

}

