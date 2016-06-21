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
class es600_base_locgic : public QObject
{
    Q_OBJECT
public:
    explicit es600_base_locgic(QObject *parent = 0);

signals:

public slots:
};

#endif // ES600_BASE_LOCGIC_H
