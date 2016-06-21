#ifndef GEFRANSEVEN_BASE_LOGIC_H
#define GEFRANSEVEN_BASE_LOGIC_H

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
#include <QWebEngineView>
#endif
#include <QSqlDatabase>
#include <QSqlQuery>

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
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    QWebPage webpage;
    QWebElement document;
    QWebElement first_document;
    QWebElementCollection documents;
#else
    QWebEnginePage *basepage;
    QWebEnginePage *optionpage1;
    int pageloadfinish_length = 0;
    QString webenginenamestr;
#endif

    QSqlDatabase remotedb;
    void requst_read_value(QString ip, QString address);
    void url_bnrbaseloop();
signals:

public slots:
    void managerfinished(QNetworkReply *reply);
};

#endif // GEFRANSEVEN_BASE_LOGIC_H
