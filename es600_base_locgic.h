#ifndef ES600_BASE_LOCGIC_H
#define ES600_BASE_LOCGIC_H

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
#include "es600_modbus_thread.h"
#include "simplecrypt.h"

#include <QModbusDataUnit>
#include <QModbusTcpClient>


//modbus 주소값 매크로
#define mb_object_count 1036
#define mb_production_count 4006
#define mb_run_mode 1026
#define mb_warning_flag 38
#define mb_mold_name 56 //글자 56~61
#define mb_cabity 1052

#define mb_temp1_set 434
#define mb_temp2_set 436
#define mb_temp3_set 438
#define mb_temp4_set 440
#define mb_temp5_set 442
#define mb_temp6_set 444
#define mb_temp7_set 446
#define mb_temp8_set 448
#define mb_temp9_set 474
#define mb_temp10_set 476
#define mb_temp11_set 478
#define mb_temp12_set 480
#define mb_temp13_set 482
#define mb_temp14_set 484
#define mb_temp15_set 486
#define mb_temp16_set 488


#define mb_temp1_up 4520
#define mb_temp2_up 4522
#define mb_temp3_up 4524
#define mb_temp4_up 4526
#define mb_temp5_up 4528
#define mb_temp6_up 4530
#define mb_temp7_up 4532
#define mb_temp8_up 4534
#define mb_temp9_up 4420
#define mb_temp10_up 4422
#define mb_temp11_up 4424
#define mb_temp12_up 4426
#define mb_temp13_up 4428
#define mb_temp14_up 4430
#define mb_temp15_up 4432
#define mb_temp16_up 4434


#define mb_temp1_down 4540
#define mb_temp2_down 4542
#define mb_temp3_down 4544
#define mb_temp4_down 4546
#define mb_temp5_down 4548
#define mb_temp6_down 4550
#define mb_temp7_down 4552
#define mb_temp8_down 4554
#define mb_temp9_down 4440
#define mb_temp10_down 4442
#define mb_temp11_down 4444
#define mb_temp12_down 4446
#define mb_temp13_down 4448
#define mb_temp14_down 4450
#define mb_temp15_down 4452
#define mb_temp16_down 4454


#define mb_temp1_onoff 4500
#define mb_temp2_onoff 4502
#define mb_temp3_onoff 4504
#define mb_temp4_onoff 4506
#define mb_temp5_onoff 4508
#define mb_temp6_onoff 4510
#define mb_temp7_onoff 4512
#define mb_temp8_onoff 4514
#define mb_temp9_onoff 4400
#define mb_temp10_onoff 4402
#define mb_temp11_onoff 4404
#define mb_temp12_onoff 4406
#define mb_temp13_onoff 4408
#define mb_temp14_onoff 4410
#define mb_temp15_onoff 4412
#define mb_temp16_onoff 4414

#define mb_temp1_real 1056
#define mb_temp2_real 1058
#define mb_temp3_real 1060
#define mb_temp4_real 1062
#define mb_temp5_real 1064
#define mb_temp6_real 1066
#define mb_temp7_real 1068
#define mb_temp8_real 1070
#define mb_temp9_real 1072
#define mb_temp10_real 1074
#define mb_temp11_real 1076
#define mb_temp12_real 1078
#define mb_temp13_real 1080
#define mb_temp14_real 1082
#define mb_temp15_real 1084
#define mb_temp16_real 1086

#define mb_actstatus 20
#define mb_SHOTDATA_count 812
#define mb_currenttotalcount 4006
#define mb_SHOTDATA_fooldata 814
#define mb_SHOTDATA_fillingtime 816
#define mb_SHOTDATA_plasticizing_time 820
#define mb_SHOTDATA_cycle_time 822
#define mb_SHOTDATA_cushion_position 826
#define mb_SHOTDATA_plasticizing_position 830
#define mb_SHOTDATA_temp1 848
#define mb_SHOTDATA_temp2 850
#define mb_SHOTDATA_temp3 852
#define mb_SHOTDATA_temp4 854
#define mb_SHOTDATA_temp5 856
#define mb_SHOTDATA_temp6 858
#define mb_SHOTDATA_temp7 860
#define mb_SHOTDATA_temp8 862
#define mb_SHOTDATA_oil_temp 1070

#define mb_moldname1 56
#define mb_moldname2 57
#define mb_moldname3 58
#define mb_moldname4 59
#define mb_moldname5 60

#define mb_injstep 770
#define mb_hldstep 772

#define mb_injVelocity1 114
#define mb_injVelocity2 116
#define mb_injVelocity3 118
#define mb_injVelocity4 120
#define mb_injVelocity5 122
#define mb_injVelocity6 124

#define mb_injPressure1 154
#define mb_injPressure2 156
#define mb_injPressure3 158
#define mb_injPressure4 160
#define mb_injPressure5 162
#define mb_injPressure6 164

#define mb_injPosition1 194
#define mb_injPosition2 196
#define mb_injPosition3 198
#define mb_injPosition4 200
#define mb_injPosition5 202
#define mb_injPosition6 204

#define mb_hldPressure1 238
#define mb_hldPressure2 240
#define mb_hldPressure3 242

#define mb_hldTime1 258
#define mb_hldTime2 260
#define mb_hldTime3 262

#define mb_hldVel1 278
#define mb_hldVel2 280
#define mb_hldVel3 282

#define mb_chgPosition1 298
#define mb_chgPosition2 300
#define mb_chgPosition3 302
#define mb_chgPosition4 304

#define mb_chgSpeed1 338
#define mb_chgSpeed2 340
#define mb_chgSpeed3 342
#define mb_chgSpeed4 344

#define mb_backPressure1 378
#define mb_backPressure2 380
#define mb_backPressure3 382
#define mb_backPressure4 384

#define mb_suckbackPosition2 420

#define mb_suckbackSpeed1 426
#define mb_suckbackSpeed2 428

#define mb_tempues1 4500
#define mb_tempues2 4502
#define mb_tempues3 4504
#define mb_tempues4 4506
#define mb_tempues5 4508
#define mb_tempues6 4510
#define mb_tempues7 4512

#define mb_tempset1 434
#define mb_tempset2 436
#define mb_tempset3 438
#define mb_tempset4 440
#define mb_tempset5 442
#define mb_tempset6 444
#define mb_tempset7 446
#define mb_oilset 448

#define mb_moldtempuse1 4400
#define mb_moldtempuse2 4402
#define mb_moldtempuse3 4404
#define mb_moldtempuse4 4406
#define mb_moldtempuse5 4408
#define mb_moldtempuse6 4410
#define mb_moldtempuse7 4412
#define mb_moldtempuse8 4414

#define mb_moldtempset1 474
#define mb_moldtempset2 476
#define mb_moldtempset3 478
#define mb_moldtempset4 480
#define mb_moldtempset5 482
#define mb_moldtempset6 484
#define mb_moldtempset7 486
#define mb_moldtempset8 488

#define mb_injtime 514
#define mb_cooltime 516
#define mb_chgtime 518


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
    QSqlDatabase es600db;
    QSqlDatabase litedb;
    QVector<int> addrlist;

    int temp_set_atnumber;
    int temp_up_atnumber;
    int temp_down_atnumber;
    int temp_real_atnumber;
    int temp_onoff_atnumber;
    QModbusTcpClient *qctx;



    int typeDB;
    bool queryresult;
    int before_shotcount;
    int current_shotcount;




    void es600_base_loop();

    void TB_REC_save();

    void TB_current_update();



signals:

public slots:
    void slot_statue_update(bool statue);
    void modbudread_ready();
    void modbusstatue_change(int state);
};

#endif // ES600_BASE_LOCGIC_H
