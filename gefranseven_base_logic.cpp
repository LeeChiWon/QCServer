#include "gefranseven_base_logic.h"
#include "mslotitem.h"

gefranseven_base_logic::gefranseven_base_logic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
}

bool gefranseven_base_logic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,gefranvalue *>;
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerfinished(QNetworkReply*)));
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    requast.setUrl(url);
    manager.get(requast);
#endif
}


void gefranseven_base_logic::managerfinished(QNetworkReply *reply){
    QByteArray temp_data;
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    temp_data = reply->readAll();
    qDebug()<<temp_data;
    if(temp_data.indexOf("PLC Realtime monitoring")>0){
        qDebug()<<"this town";
        QString ip = parent_item->ip->text();
        QString seturl = QString("http://%1/cgi-bin/SevenCgiLib.out").arg(ip);
        requast.setUrl(QUrl(seturl));
        manager.get(requast);

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
        webpage.mainFrame()->setHtml(temp_data);
        documents = webpage.mainFrame()->findAllElements("input");
        for(int i=0;i<documents.count();i++){
             document = documents.at(i);
             QString name = document.attribute("name");
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
    }

    delete reply;

}
