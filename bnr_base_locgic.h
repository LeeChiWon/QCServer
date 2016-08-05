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
#include "simplecrypt.h"
#include <QMessageAuthenticationCode>
#include "globalheader.h"
#include <alrammap_data.h>
#define GEN_ALRAM_MAX 130
#define ALRAM_MAX 900
#define HtGr_ALRAM_START 140
#define Htzo_ALRAM_START 200
#define Ax_ALRAM_START 600

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
    QMap<QString,alrammap_data *> *alrammap;
    QNetworkAccessManager manager;
    QNetworkRequest requast;
    QSqlDatabase litedb;
    QSqlDatabase remotedb;
    QWaitCondition waitcondition;
    QMutex mutex;
    QMutex httpmutex;
    bnr_moudle_thread *moudle_thread;

    void url_bnrbaseloop();
    void TA_REC_SAVE();
    void TA_current_update();
    void TE_current_update();
    void TE_REC_SAVE();
    void DL_current_update();
    void DL_REC_SAVE();
    void alram_update();
    QString get_mold_name();
    int before_prod_actpcs;
    int current_prod_actpcs;
    ~Bnr_base_locgic();


signals:
    void sig_requst_read_value(QString ip,QString address);

public slots:
    void requst_read_value(QString ip, QString address);
    void managerfinished(QNetworkReply *reply);
    void pageloadfinish(bool);

};


#endif // BNR_BASE_LOCGIC_H
