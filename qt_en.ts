<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">
<context>
    <name>Bnr_base_locgic</name>
    <message>
        <location filename="bnr_base_locgic.cpp" line="183"/>
        <source>auto</source>
        <translation>auto</translation>
    </message>
    <message>
        <location filename="bnr_base_locgic.cpp" line="185"/>
        <source>helfauto</source>
        <translation>helfauto</translation>
    </message>
    <message>
        <location filename="bnr_base_locgic.cpp" line="187"/>
        <source>munual</source>
        <translation>munual</translation>
    </message>
    <message>
        <location filename="bnr_base_locgic.cpp" line="189"/>
        <source>mold</source>
        <translation>mold</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="14"/>
        <source>MainWindow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="23"/>
        <source>IP</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="45"/>
        <source>999.999.999.999</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="67"/>
        <source>추가</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="91"/>
        <source>기계이름</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="108"/>
        <source>제거</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="138"/>
        <source>서버시작</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="149"/>
        <source>서버중지</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="210"/>
        <source>Menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="232"/>
        <source>start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="241"/>
        <source>setting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="72"/>
        <source>ip 가입력되지 않았습니다. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="75"/>
        <source>기계이이름  등록되지 않았습니다.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="83"/>
        <source>이미 등록된 기계이름입니다.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="210"/>
        <source>CREATE TABLE `temp_table` (`machine_name` VARCHAR(50) NULL DEFAULT NULL,`temp1_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp1_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp1_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp1_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp1_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도1&apos; COMMENT &apos;온도이름 &apos; COLLATE &apos;utf8_bin&apos;,`temp1_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp2_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp2_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp2_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp2_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp2_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도2&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp2_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp3_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp3_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp3_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp3_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp3_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도3&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp3_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp4_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp4_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp4_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp4_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp4_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도4&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp4_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp5_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp5_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp5_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp5_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp5_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도5&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp5_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp6_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp6_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp6_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp6_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp6_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도6&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp6_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp7_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp7_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp7_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp7_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp7_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도7&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp7_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp8_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp8_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp8_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp8_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp8_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도8&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp8_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp9_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp9_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp9_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp9_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp9_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도9&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp9_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp10_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp10_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp10_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp10_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp10_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도10&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp10_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp11_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp11_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp11_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp11_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp11_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도11&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp11_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp12_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp12_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp12_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp12_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp12_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도12&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp12_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp13_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp13_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp13_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp13_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp13_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도13&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp13_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp14_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp14_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp14_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp14_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp14_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도14&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp14_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp15_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp15_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp15_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp15_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp15_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도15&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp15_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp16_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp16_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp16_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp16_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp16_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도16&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp16_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp17_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp17_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp17_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp17_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp17_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도17&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp17_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp18_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp18_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp18_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp18_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp18_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도18&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp18_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp19_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp19_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp19_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp19_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp19_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도19&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp19_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp20_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp20_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp20_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp20_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp20_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도20&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp20_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`temp21_set` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;설정온도&apos;,`temp21_up` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;상한온도&apos;,`temp21_down` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;하한온도&apos;,`temp21_real` DOUBLE NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;실제값&apos;,`temp21_name` VARCHAR(50) NOT NULL DEFAULT &apos;온도21&apos; COMMENT &apos;온도이름&apos; COLLATE &apos;utf8_bin&apos;,`temp21_onoff` INT NOT NULL DEFAULT &apos;0&apos;,`modift_version` INT(11) NOT NULL DEFAULT &apos;0&apos; COMMENT &apos;버전&apos;,UNIQUE INDEX `machine_name` (`machine_name`))COLLATE=&apos;utf8_bin&apos;ENGINE=InnoDB;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Serversetform</name>
    <message>
        <location filename="serversetform.ui" line="14"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="34"/>
        <source>USERNAME</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="41"/>
        <source>서버 DBname</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="48"/>
        <source>서버Port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="55"/>
        <source>99999</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="65"/>
        <source>999.999.999.999</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="75"/>
        <source>서버IP</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="82"/>
        <source>적용</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="serversetform.ui" line="89"/>
        <source>USERPASSWORD</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>mslotitem</name>
    <message>
        <location filename="mslotitem.cpp" line="15"/>
        <source>setup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mslotitem.cpp" line="17"/>
        <source>&lt;img src=&quot;:/icon/icon/light-bulb_red.png&quot;&gt;  disconnect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mslotitem.cpp" line="22"/>
        <source>&lt;img src=&quot;:/icon/icon/stop.png&quot;&gt;  STOP</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
