#ifndef BNR_BASE_LOCGIC_H
#define BNR_BASE_LOCGIC_H

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
#include "bnr_moudle_thread.h"

class BNRvalue {
public:
    QString name;
    QString value;
    BNRvalue(){

    }
    BNRvalue(QString name,QString value){
        this->name=name;
        this->value=value;
    }
};

class Bnr_base_locgic : public QObject
{
    Q_OBJECT
public:
    explicit Bnr_base_locgic(QObject *parentmslot,QObject *parent = 0);
    bool initflag;
    bool init();
    void loop();
    QObject *parentmslot;
    QMap<QString,BNRvalue *> *datamap; //<name,value>
    QNetworkAccessManager manager;
    QNetworkRequest requast;
    QSqlDatabase litedb;
    QSqlDatabase remotedb;
    QWaitCondition waitcondition;
    QMutex mutex;
    bnr_moudle_thread *moudle_thread;
    void requst_read_value(QString ip, QString address);
    void url_bnrbaseloop();
    ~Bnr_base_locgic();


signals:

public slots:
    void managerfinished(QNetworkReply *reply);
    void pageloadfinish(bool);

};


#endif // BNR_BASE_LOCGIC_H
