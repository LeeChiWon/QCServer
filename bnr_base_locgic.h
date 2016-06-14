#ifndef BNR_BASE_LOCGIC_H
#define BNR_BASE_LOCGIC_H

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
#else
#include <QWebEnginePage>
#endif
#include <QSqlDatabase>
#include <QSqlQuery>

class BNRvalue {
public:
    QString name;
    QString value;
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
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    QWebPage webpage;
    QWebElement document;
    QWebElement first_document;
    QWebElementCollection documents;
#else
    QWebEnginePage basepage;
    QWebEnginePage optionpage1;
    int pageloadfinish_length = 0;
    QString webenginenamestr;

#endif

    QSqlDatabase remotedb;

    void requst_read_value(QString ip, QString address);
    void url_bnrbaseloop();
    void url_tac1xx11warningloop();

signals:

public slots:
    void managerfinished(QNetworkReply *reply);
    void pageloadfinish(bool);

};


#endif // BNR_BASE_LOCGIC_H
