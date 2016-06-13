#ifndef BNR_BASE_LOCGIC_H
#define BNR_BASE_LOCGIC_H

#include <QObject>
#include <QFtp>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QSqlDatabase>
#include <QSqlQuery>

class BNRvalue {
public:
    QString name;
    QString value;
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
    QWebPage webpage;
    QWebElement document;
    QWebElement first_document;
    QWebElementCollection documents;
    QSqlDatabase remotedb;

    void requst_read_value(QString ip, QString address);
    void url_bnrbaseloop();
    void url_tac1xx11warningloop();

signals:

public slots:
    void managerfinished(QNetworkReply *reply);

};


#endif // BNR_BASE_LOCGIC_H
