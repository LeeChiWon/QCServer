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


TARGET = QCServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mslotitem.cpp \
    serversetform.cpp \
    bnr_base_locgic.cpp

HEADERS  += mainwindow.h \
    mslotitem.h \
    serversetform.h \
    globalheader.h \
    bnr_base_locgic.h

FORMS    += mainwindow.ui \
    serversetform.ui

RESOURCES += \
    Res.qrc

DISTFILES +=

