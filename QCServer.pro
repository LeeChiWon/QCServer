#-------------------------------------------------
#
# Project created by QtCreator 2016-05-30T22:15:48
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_VERSION, 5.6): QT += webkit webkitwidgets
greaterThan(QT_VERSION, 5.6): QT += webenginewidgets

TRANSLATIONS = qt_ko.ts qt_en.ts

#modbuslib use need lib
win32{
LIBS += -lws2_32
}
TARGET = QCServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mslotitem.cpp \
    serversetform.cpp \
    bnr_base_locgic.cpp \
    es600_base_locgic.cpp \
    gefranseven_base_logic.cpp \
    modbus.c \
    modbus-data.c \
    modbus-rtu.c \
    modbus-tcp.c \
    es600_modbus_thread.cpp \
    bnr_moudle_thread.cpp \
    gefranseven_moudle_thread.cpp

HEADERS  += mainwindow.h \
    mslotitem.h \
    serversetform.h \
    globalheader.h \
    bnr_base_locgic.h \
    es600_base_locgic.h \
    gefranseven_base_logic.h \
    modbus.h \
    modbus-private.h \
    modbus-rtu.h \
    modbus-rtu-private.h \
    modbus-tcp.h \
    modbus-tcp-private.h \
    modbus-version.h \
    config.h \
    es600_modbus_thread.h \
    bnr_moudle_thread.h \
    gefranseven_moudle_thread.h

FORMS    += mainwindow.ui \
    serversetform.ui

RESOURCES += \
    Res.qrc

DISTFILES +=

