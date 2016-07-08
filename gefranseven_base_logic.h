#ifndef GEFRANSEVEN_BASE_LOGIC_H
#define GEFRANSEVEN_BASE_LOGIC_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWaitCondition>
#include <QMutex>
#include "gefranseven_moudle_thread.h"


class gefranvalue {
public:
    QString name;
    QString value;
    gefranvalue(){

    }
    gefranvalue(QString name,QString value){
        this->name=name;
        this->value=value;
    }
};

class gefranseven_base_logic : public QObject
{
    Q_OBJECT
public:
    explicit gefranseven_base_logic(QObject *parentmslot,QObject *parent = 0);
    bool initflag;
    bool init();
    void loop();
    QObject *parentmslot;
    QMap<QString,gefranvalue *> *datamap; //<name,value>
    QNetworkAccessManager manager;
    QNetworkRequest requast;
    QSqlDatabase litedb;
    QSqlDatabase remotedb;
    QWaitCondition waitcondition;
    QMutex mutex;
    gefranseven_moudle_thread *moudle_thread;
    void requst_read_value(QString ip, QString address);
    void url_gefranbaseloop();
signals:

public slots:
    void managerfinished(QNetworkReply *reply);
};

#endif // GEFRANSEVEN_BASE_LOGIC_H
