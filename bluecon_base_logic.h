#ifndef BLUECON_BASE_LOGIC_H
#define BLUECON_BASE_LOGIC_H

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
#include "bluecon_modbus_thread.h"
#include <QTcpSocket>
#include "simplecrypt.h"
#include "alrammap_data.h"
#include <QModbusDataUnit>
#include <QModbusTcpClient>

#define bmb_object_count 1036
#define bmb_production_count 1038
#define bmb_run_mode 1026
#define bmb_warning_flag 32
#define bmb_cabity 1052

#define bmb_moldname1 56
#define bmb_moldname2 57
#define bmb_moldname3 58
#define bmb_moldname4 59
#define bmb_moldname5 60
#define bmb_moldname6 62
#define bmb_moldname7 64
#define bmb_moldname8 66
#define bmb_moldname9 68
#define bmb_moldname10 70

#define bmb_injVelocity1 114
#define bmb_injVelocity2 116
#define bmb_injVelocity3 118
#define bmb_injVelocity4 120
#define bmb_injVelocity5 122
#define bmb_injVelocity6 124
#define bmb_injVelocity7 126
#define bmb_injVelocity8 128
#define bmb_injVelocity9 130
#define bmb_injVelocity10 132

#define bmb_injPressure1 154
#define bmb_injPressure2 156
#define bmb_injPressure3 158
#define bmb_injPressure4 160
#define bmb_injPressure5 162
#define bmb_injPressure6 164
#define bmb_injPressure7 166
#define bmb_injPressure8 168
#define bmb_injPressure9 170
#define bmb_injPressure10 172

#define bmb_injPosition1 194
#define bmb_injPosition2 196
#define bmb_injPosition3 198
#define bmb_injPosition4 200
#define bmb_injPosition5 202
#define bmb_injPosition6 204
#define bmb_injPosition7 206
#define bmb_injPosition8 208
#define bmb_injPosition9 210
#define bmb_injPosition10 212

#define bmb_sov_time 234
#define bmb_sov_position 236

#define bmb_hldPressure1 238
#define bmb_hldPressure2 240
#define bmb_hldPressure3 242
#define bmb_hldPressure4 244
#define bmb_hldPressure5 246

#define bmb_hldTime1 258
#define bmb_hldTime2 260
#define bmb_hldTime3 262
#define bmb_hldTime4 264
#define bmb_hldTime5 266

#define bmb_hldVel1 278
#define bmb_hldVel2 280
#define bmb_hldVel3 282
#define bmb_hldVel4 284
#define bmb_hldVel5 286

#define bmb_chgPosition1 298
#define bmb_chgPosition2 300
#define bmb_chgPosition3 302
#define bmb_chgPosition4 304

#define bmb_chgSpeed1 338
#define bmb_chgSpeed2 340
#define bmb_chgSpeed3 342
#define bmb_chgSpeed4 344

#define bmb_backPressure1 378
#define bmb_backPressure2 380
#define bmb_backPressure3 382
#define bmb_backPressure4 384

#define bmb_suckbackPosition1 418
#define bmb_suckbackPosition2 420

#define bmb_suckbackSpeed1 426
#define bmb_suckbackSpeed2 428

#define bmb_temp1_set 434
#define bmb_temp2_set 436
#define bmb_temp3_set 438
#define bmb_temp4_set 440
#define bmb_temp5_set 442
#define bmb_temp6_set 444
#define bmb_temp7_set 446
#define bmb_temp8_set 448
#define bmb_temp9_set 474
#define bmb_temp10_set 476
#define bmb_temp11_set 478
#define bmb_temp12_set 480
#define bmb_temp13_set 482
#define bmb_temp14_set 484
#define bmb_temp15_set 486
#define bmb_temp16_set 488

#define bmb_INJETM 514
#define bmb_COOLTM 516
#define bmb_SPR2TM 518
#define bmb_CHDLTM 520

#define bmb_injstep 770
#define bmb_hldstep 772


#define bmb_SHOTDATA_fooldata 814
#define bmb_SHOTDATA_inctiontime 816
#define bmb_SHOTDATA_fillingtime 818
#define bmb_SHOTDATA_plasticizing_time 820
#define bmb_SHOTDATA_cycle_time 822
#define bmb_SHOTDATA_clamp_close_time 824
#define bmb_SHOTDATA_cushion_position 826
#define bmb_SHOTDATA_switch_over_position 828
#define bmb_SHOTDATA_plasticizing_position 830
#define bmb_SHOTDATA_clamp_open_position 832
#define bmb_SHOTDATA__max_injection_speed 834
#define bmb_SHOTDATA__max_screw_RPM 836
#define bmb_SHOTDATA__average_screw_RPM 838
#define bmb_SHOTDATA__max_injection_pressure 840
#define bmb_SHOTDATA__max_switch_over_pressure 842
#define bmb_SHOTDATA__max_back_pressure 844
#define bmb_SHOTDATA__average_back_pressure 846


#define bmb_alrammap1 1400
#define bmb_alrammap2 1402
#define bmb_alrammap3 1404
#define bmb_alrammap4 1406
#define bmb_alrammap5 1408
#define bmb_alrammap6 1410
#define bmb_alrammap7 1412
#define bmb_alrammap8 1414
#define bmb_alrammap9 1416
#define bmb_alrammap10 1418


#define bmb_temp1_real 1056
#define bmb_temp2_real 1058
#define bmb_temp3_real 1060
#define bmb_temp4_real 1062
#define bmb_temp5_real 1064
#define bmb_temp6_real 1066
#define bmb_temp7_real 1068
#define bmb_temp8_real 1070
#define bmb_temp9_real 1072
#define bmb_temp10_real 1074
#define bmb_temp11_real 1076
#define bmb_temp12_real 1078
#define bmb_temp13_real 1080
#define bmb_temp14_real 1082
#define bmb_temp15_real 1084
#define bmb_temp16_real 1086


class blueconvalue {
public:
    QString name;
    QString value;
    blueconvalue(){

    }
    blueconvalue(QString name,QString value){
        this->name=name;
        this->value=value;
    }
};


class bluecon_base_logic : public QObject
{
    Q_OBJECT
public:
    explicit bluecon_base_logic(QObject *parentmslot,QObject *parent = 0);
    bool initflag;
    bool init();
    void loop();
    QObject *parentmslot;
    QMap<QString,blueconvalue *> *datamap; //<name,value>
    QMap<QString,alrammap_data *> *alrammap;
    QSqlDatabase litedb;
    QSqlDatabase remotedb;
    QWaitCondition waitcondition;
    QMutex mutex;
    bluecon_modbus_thread *modbus_thread;
    QString result_mold_name;
    QString temp_mold_info;
    QVector<int> addrlist;

    int connect_time_loop;

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

    void REC_save();

    void current_update();

    QString get_mold_name();
    QString get_program_name();
    void alram_update();

    int modbuscount;

signals:

public slots:
        void bluecon_base_loop();
        void slot_statue_update(bool statue);
        void modbusread_ready();
        void modbusstatue_change(int state);
};

#endif // BLUECON_BASE_LOGIC_H
