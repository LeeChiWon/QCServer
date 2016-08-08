#ifndef GEFRANSEVEN_BASE_LOGIC_H
#define GEFRANSEVEN_BASE_LOGIC_H

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
#include "gefranseven_moudle_thread.h"
#include <QTcpSocket>
#include "simplecrypt.h"
#include "alrammap_data.h"
#include <QModbusDataUnit>
#include <QModbusTcpClient>

#define gmb_temp1_down 3002
#define gmb_temp2_down 3004
#define gmb_temp3_down 3006
#define gmb_temp4_down 3008
#define gmb_temp5_down 3010
#define gmb_temp6_down 3012
#define gmb_temp7_down 3014

#define gmb_temp1_up 3018
#define gmb_temp2_up 3020
#define gmb_temp3_up 3022
#define gmb_temp4_up 3024
#define gmb_temp5_up 3026
#define gmb_temp6_up 3028
#define gmb_temp7_up 3030

#define gmb_oil_down 3032
#define gmb_oil_up 3034

#define gmb_tempuse1 3038
#define gmb_tempuse2 3040
#define gmb_tempuse3 3042
#define gmb_tempuse4 3044
#define gmb_tempuse5 3046
#define gmb_tempuse6 3048
#define gmb_tempuse7 3050

#define gmb_oiluse 3052
#define gmb_oil_real 3054

#define gmb_temp1_real 3056
#define gmb_temp2_real 3058
#define gmb_temp3_real 3060
#define gmb_temp4_real 3062
#define gmb_temp5_real 3064
#define gmb_temp6_real 3066
#define gmb_temp7_real 3068

#define gmb_moldtempset1 3070
#define gmb_moldtempset2 3072
#define gmb_moldtempset3 3074
#define gmb_moldtempset4 3076
#define gmb_moldtempset5 3078
#define gmb_moldtempset6 3080
#define gmb_moldtempset7 3082
#define gmb_moldtempset8 3084

#define gmb_moldtemp1_down 3086
#define gmb_moldtemp2_down 3088
#define gmb_moldtemp3_down 3090
#define gmb_moldtemp4_down 3092
#define gmb_moldtemp5_down 3094
#define gmb_moldtemp6_down 3096
#define gmb_moldtemp7_down 3098
#define gmb_moldtemp8_down 3100

#define gmb_moldtemp1_up 3102
#define gmb_moldtemp2_up 3104
#define gmb_moldtemp3_up 3106
#define gmb_moldtemp4_up 3108
#define gmb_moldtemp5_up 3110
#define gmb_moldtemp6_up 3112
#define gmb_moldtemp7_up 3114
#define gmb_moldtemp8_up 3116

#define gmb_moldtempuse1 3118
#define gmb_moldtempuse2 3120
#define gmb_moldtempuse3 3122
#define gmb_moldtempuse4 3124
#define gmb_moldtempuse5 3126
#define gmb_moldtempuse6 3128
#define gmb_moldtempuse7 3130
#define gmb_moldtempuse8 3132

#define gmb_moldtemp1_real 3134
#define gmb_moldtemp2_real 3136
#define gmb_moldtemp3_real 3138
#define gmb_moldtemp4_real 3140
#define gmb_moldtemp5_real 3142
#define gmb_moldtemp6_real 3144
#define gmb_moldtemp7_real 3146
#define gmb_moldtemp8_real 3148

#define gmb_TOTPERCENT 3152
#define gmb_TOTPR 3154
#define gmb_TOTPS 3156
#define gmb_CYCLCT 3158
#define gmb_CAVITY 3160

#define gmb_mANUAL 3162
#define gmb_lSemiAuto 3164
#define gmb_lFullAuto 3166
#define gmb_MADJSL 3168

#define gmb_dfl_Alarm 3170
#define gmb_dmt_RunMode 3172
#define gmb_dfl_Motor 3174
#define gmb_dfl_Heater 3176

#define gmb_WorkFile_1 3178
#define gmb_WorkFile_2 3180
#define gmb_WorkFile_3 3182
#define gmb_WorkFile_4 3184
#define gmb_WorkFile_5 3186
#define gmb_WorkFile_6 3188
#define gmb_WorkFile_7 3190
#define gmb_WorkFile_8 3192
#define gmb_WorkFile_9 3194
#define gmb_WorkFile_10 3196
#define gmb_WorkFile_11 3198
#define gmb_WorkFile_12 3200
#define gmb_WorkFile_13 3202
#define gmb_WorkFile_14 3204
#define gmb_WorkFile_15 3206
#define gmb_WorkFile_16 3208
#define gmb_WorkFile_17 3210
#define gmb_WorkFile_18 3212
#define gmb_WorkFile_19 3214
#define gmb_WorkFile_20 3216
#define gmb_WorkFile_21 3218
#define gmb_WorkFile_22 3220
#define gmb_WorkFile_23 3222
#define gmb_WorkFile_24 3224
#define gmb_WorkFile_25 3226
#define gmb_WorkFile_26 3228
#define gmb_WorkFile_27 3230
#define gmb_WorkFile_28 3232
#define gmb_WorkFile_29 3234

#define gmb_MCLSTK 3236
#define gmb_INJETK 3238
#define gmb_CHRGTK 3240
#define gmb_CYCLTK 3242
#define gmb_MCLSTK1 3244
#define gmb_CUSPOS 3246
#define gmb_HLDPOS 3248
#define gmb_CRGPOS 3250
#define gmb_MDOPOS 3252
#define gmb_MAXINS 3254
#define gmb_MAXRPM 3256
#define gmb_AVRRPM 3258
#define gmb_MAXINP 3260
#define gmb_HLDPRS 3262
#define gmb_MAXBPR 3264
#define gmb_AVRBPR 3266

#define gmb_shotdata_injectiontime 3268
#define gmb_shotdata_ChargeTime 3270
#define gmb_shotdata_CycleTime 3272
#define gmb_shotdata_MoldCloseTime 3274
#define gmb_shotdata_CushionPos 3276
#define gmb_shotdata_HoldStartPos 3278
#define gmb_shotdata_ChargeEndPos 3280
#define gmb_shotdata_MaxInjectSpd 3282
#define gmb_shotdata_MaxChargeSpd 3284
#define gmb_shotdata_AvgChargeSpd 3286
#define gmb_shotdata_MaxInjectPrs 3288
#define gmb_shotdata_HoldPrs 3290
#define gmb_shotdata_MaxBackPrs 3292
#define gmb_shotdata_AvgBackPrs 3294
#define gmb_shotdata_Temperature1 3296
#define gmb_shotdata_Temperature2 3298
#define gmb_shotdata_Temperature3 3300
#define gmb_shotdata_Temperature4 3302
#define gmb_shotdata_Temperature5 3304
#define gmb_shotdata_Temperature6 3306
#define gmb_shotdata_Temperature7 3308
#define gmb_shotdata_Temperature8 3310
#define gmb_shotdata_MoldOpenPos 3312
#define gmb_shotdata_CycleCounter 3314

#define gmb_SetInjectionStep 3316
#define gmb_SetHoldingStep 3318

#define gmb_sp_Inject_0 3320
#define gmb_sp_Inject_1 3322
#define gmb_sp_Inject_2 3324
#define gmb_sp_Inject_3 3326
#define gmb_sp_Inject_4 3328
#define gmb_sp_Inject_5 3330

#define gmb_pr_Inject_0 3332
#define gmb_pr_Inject_1 3334
#define gmb_pr_Inject_2 3336
#define gmb_pr_Inject_3 3338
#define gmb_pr_Inject_4 3340
#define gmb_pr_Inject_5 3342

#define gmb_po_Inject_0 3344
#define gmb_po_Inject_1 3346
#define gmb_po_Inject_2 3348
#define gmb_po_Inject_3 3350
#define gmb_po_Inject_4 3352
#define gmb_po_Inject_5 3354

#define gmb_sp_Holdp_0 3356
#define gmb_sp_Holdp_1 3358
#define gmb_sp_Holdp_2 3360

#define gmb_pr_Holdp_0 3362
#define gmb_pr_Holdp_1 3364
#define gmb_pr_Holdp_2 3366

#define gmb_ts_Holdp_0 3368
#define gmb_ts_Holdp_1 3370
#define gmb_ts_Holdp_2 3372

#define gmb_sp_Charge_0 3374
#define gmb_sp_Charge_1 3376
#define gmb_sp_Charge_2 3378
#define gmb_sp_Charge_3 3380

#define gmb_pr_Charge_0 3382
#define gmb_pr_Charge_1 3384
#define gmb_pr_Charge_2 3386
#define gmb_pr_Charge_3 3388

#define gmb_po_Charge_0 3390
#define gmb_po_Charge_1 3392
#define gmb_po_Charge_2 3394
#define gmb_po_Charge_3 3396

#define gmb_bp_Charge_0 3398
#define gmb_bp_Charge_1 3400
#define gmb_bp_Charge_2 3402
#define gmb_bp_Charge_3 3404

#define gmb_sp_Suckb_0 3406
#define gmb_sp_Suckb_1 3408

#define gmb_po_Suckb_0 3410
#define gmb_po_Suckb_2 3412

#define gmb_SPR2TM 3414
#define gmb_COOLTM 3416
#define gmb_CHDLTM 3418

#define gmb_alramnumber0 3420
#define gmb_alramnumber1 3422
#define gmb_alramnumber2 3424
#define gmb_alramnumber3 3426
#define gmb_alramnumber4 3428
#define gmb_alramnumber5 3430
#define gmb_alramnumber6 3432
#define gmb_alramnumber7 3434
#define gmb_alramnumber8 3436
#define gmb_alramnumber9 3438

#define gmb_duv_TopLineNG_0 3440
#define gmb_duv_TopLineNG_1 3442
#define gmb_duv_TopLineNG_2 3444
#define gmb_duv_TopLineNG_3 3446
#define gmb_duv_TopLineNG_4 3448
#define gmb_duv_TopLineNG_5 3450

#define gmb_dts_Holdp_0 3452
#define gmb_dts_Holdp_1 3454
#define gmb_dts_Holdp_2 3456

#define gmb_uv_HeatSet_1 3458
#define gmb_uv_HeatSet_2 3460
#define gmb_uv_HeatSet_3 3462
#define gmb_uv_HeatSet_4 3464
#define gmb_uv_HeatSet_5 3466
#define gmb_uv_HeatSet_6 3468
#define gmb_uv_HeatSet_7 3470

#define gmb_uv_OilSet 3472

#define gmb_INJETM 3474

#define gmb_program_name_1 3476
#define gmb_program_name_2 3478
#define gmb_program_name_3 3480
#define gmb_program_name_4 3482
#define gmb_program_name_5 3484
#define gmb_program_name_6 3486
#define gmb_program_name_7 3488
#define gmb_program_name_8 3490
#define gmb_program_name_9 3492
#define gmb_program_name_10 3494
#define gmb_program_name_11 3496
#define gmb_program_name_12 3498
#define gmb_program_name_13 3500
#define gmb_program_name_14 3502
#define gmb_program_name_15 3504
#define gmb_program_name_16 3506
#define gmb_program_name_17 3508
#define gmb_program_name_18 3510
#define gmb_program_name_19 3512
#define gmb_program_name_20 3514

#define gmb_pr_EX_Holdp 3516


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
    QMap<QString,alrammap_data *> *alrammap;
    //QNetworkAccessManager manager;
    //QNetworkRequest requast;
    QSqlDatabase litedb;
    QSqlDatabase remotedb;
    QWaitCondition waitcondition;
    QMutex mutex;
    gefranseven_moudle_thread *moudle_thread;
    QTcpSocket *tcpsocket;
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

    void requst_read_value(QString ip, QString address);
    void url_gefranbaseloop();

    QString get_mold_name();
    QString get_program_name();
    void alram_update();
    // gefran은 리시브를 받지 않고 바로 요청 패킷을 보내면 연결을 해제 한다.
    int modbuscount;
signals:

public slots:
    void managerfinished(QNetworkReply *reply);
    void telnetreadready();
    void modbusstatue_change(int state);
    void modbusread_ready();
    void gefranseven_base_loop();
    void slot_statue_update(bool statue);

};

#endif // GEFRANSEVEN_BASE_LOGIC_H
