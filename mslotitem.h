#ifndef MSLOTITEM_H
#define MSLOTITEM_H
/*
 *
 * maintablewidget 에 들어갈 아이템
 *
 *
*/

#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QTimer>
#include "bnr_base_locgic.h"
#include "gefranseven_base_logic.h"
#include "globalheader.h"
#include "es600_base_locgic.h"
#include "bluecon_base_logic.h"
#include <QSqlError>
#include <gefransetup_popup.h>

class mslotitem : public QObject
{
    Q_OBJECT
public:
    explicit mslotitem(QString iptext,QString machinenametext,QObject *parent = 0);
    QLabel *connectlabel;
    QLabel *status;
    QLabel *ip;
    QLabel *machinename;
    QComboBox *type;
    QPushButton *setupbtn;
    QString iptext;
    QSqlDatabase remotedb;
    QSqlDatabase localdb;
    QString machinenametext;
    QTimer maintimer;
    Bnr_base_locgic *bnr_base_logic;
    gefranseven_base_logic *gefran_base_logic;
    es600_base_locgic *Es600_base_locgic;
    bluecon_base_logic *Bluecon_base_logic;
    QWidget *parent_src;
    gefransetup_popup *gefranset_popup;
    void set_connectlabel_text(QString data);
    void set_status_text(QString data);



signals:

public slots:
    void typechange(QString data);
    void maintimer_timeout();
    void setupbtn_click(bool result);


};

#endif // MSLOTITEM_H
