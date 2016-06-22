#include "gefranseven_base_logic.h"
#include "mslotitem.h"

gefranseven_base_logic::gefranseven_base_logic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
#if QT_VERSION > QT_VERSION_CHECK(5,6,0)

#endif
}

bool gefranseven_base_logic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,gefranvalue *>;


    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));



    remotedb  = QSqlDatabase::database("RemoteDB");
    initflag=true;
    return initflag;
}

void gefranseven_base_logic::loop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    requst_read_value(ip,"cgi-bin/SevenCgiLib.out");
}
void gefranseven_base_logic::requst_read_value(QString ip, QString address){
    QString url = QString("http://%1/%2").arg(ip).arg(address);
    requast.setUrl(url);
    manager.get(requast);
}
void gefranseven_base_logic::managerfinished(QNetworkReply *reply){
    QByteArray temp_data;

    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    temp_data = reply->readAll();
    if(temp_data.size()>0){
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/play-button16.png\">  connect");
        parent_item->set_status_text("<img src=\":/icon/icon/play-button16.png\">  play");
    }else {
        parent_item->set_connectlabel_text("<img src=\":/icon/icon/light-bulb_red.png\">  disconnect");
        parent_item->set_status_text("<img src=\":/icon/icon/stop.png\">  STOP");
        delete reply;
        return ;
    }
    if(temp_data.indexOf("PLC Realtime monitoring")>0){
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
        webpage.mainFrame()->setHtml(temp_data);
        documents1 = webpage.mainFrame()->findAllElements("table tr td p i");
        documents2 = webpage.mainFrame()->findAllElements("table tr td p b");
        for(int i=0;i<documents1.count();i++){
            document1 = documents1.at(i);
            QString name = document1.toPlainText();
            document2 = documents2.at(i);
            QString value = document2.toPlainText();
            gefranvalue *tempgefdata;
            if(!datamap->contains(name)){
                tempgefdata = new gefranvalue();
                tempgefdata->name = name;
                datamap->insert(name,tempgefdata);
            }else {
                tempgefdata = datamap->value(name);
            }
            tempgefdata->value = value;
        }
#endif
        url_gefranbaseloop();

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
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
        QByteArray postData;
        webpage.mainFrame()->setHtml(temp_data);
        documents1 = webpage.mainFrame()->findAllElements("input");
        for(int i=0;i<documents1.count();i++){
             document1 = documents1.at(i);
             QString name = document1.attribute("name");
             if((name.compare("button_getvars")==0) || (name.compare("button")==0)){
                //no cycle
             }else{
                QString data  = QString("%1=SEL-ON&").arg(name);
                postData.append(data.toLocal8Bit());
             }
        }
        postData.append("button_getvars=submit");
        QString ip = parent_item->ip->text();
        QString seturl = QString("http://%1/cgi-bin/SevenCgiLib.out").arg(ip);
        requast.setUrl(QUrl(seturl));
        requast.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");
        manager.post(requast,postData);
#endif

    }
    delete reply;
}


void gefranseven_base_logic::url_gefranbaseloop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString mancine_name = parent_item->machinename->text();
    QSqlQuery mysqlquery1(remotedb);
    //qDebug()<<datamap->value(QString("sp_Injec_0"))->value;

}
