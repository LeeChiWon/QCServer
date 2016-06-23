#ifndef ES600_BASE_LOCGIC_H
#define ES600_BASE_LOCGIC_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#endif
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWaitCondition>
#include <QMutex>
#include "es600_modbus_thread.h"
#include "modbus.h"


class es600value {
    public:
        QString name;
        QString value;
        es600value(){

        }
        es600value(QString name,QString value){
            this->name=name;
            this->value=value;
        }
};

class es600_base_locgic : public QObject
{
    Q_OBJECT
public:
    explicit es600_base_locgic(QObject *parentmslot,QObject *parent = 0);
    QObject *parentmslot;
    QMap<QString,es600value *> *datamap; //<name,value>
    bool initflag;
    bool init();
    void loop();
    QSqlDatabase remotedb;
    es600_modbus_thread *modbus_thread;
    QWaitCondition waitcondition;
    QMutex mutex;
    QString ip;
    modbus_t *ctx;


    void es600_base_loop();

signals:

public slots:
    void slot_statue_update(bool statue);


};

#endif // ES600_BASE_LOCGIC_H
