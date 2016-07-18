#include "gefranseven_base_logic.h"
#include "mslotitem.h"

gefranseven_base_logic::gefranseven_base_logic(QObject *parentmslot,QObject *parent) : QObject(parent)
{
    this->parentmslot = parentmslot;
    initflag=false;
    connect_time_loop = 0;
#if QT_VERSION > QT_VERSION_CHECK(5,6,0)

#endif
}

bool gefranseven_base_logic::init(){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯
    datamap = new QMap<QString,gefranvalue *>;
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
        qDebug()<<"gefran DB not open";

    }else {
       qDebug()<<"gefran DB open";
    }
    tcpsocket = new QTcpSocket();
    tcpsocket->connectToHost(parent_item->ip->text(),23);
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(telnetreadready()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(telnetjoinerror(QAbstractSocket::SocketError)));
    moudle_thread = new gefranseven_moudle_thread(this);
    moudle_thread->start();
    initflag=true;
    return initflag;
}



void gefranseven_base_logic::telnetjoinerror(QAbstractSocket::SocketError number){
    mslotitem *parent_item = (mslotitem *)parentmslot; //부모 위젯

}
//금형 이름 받는 로직
void gefranseven_base_logic::telnetreadready(){
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
}

void gefranseven_base_logic::loop(){
    mslotitem * parent_item = (mslotitem *)parentmslot; //부모 위젯
    QString ip = parent_item->ip->text();
    requst_read_value(ip,"cgi-bin/SevenCgiLib.out");
    //금형 이름을 받기 위함
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
    //qDebug()<<tcpsocket->state();
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
}


void gefranseven_base_logic::url_gefranbaseloop(){
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
}
