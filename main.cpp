#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QTranslator qtTranslator;
    qtTranslator.load("qt_"+QLocale::system().name(),
                      ":/lang/");
    QString ts = QString("qt_%1").arg(QLocale::system().name());

    a.installTranslator(&qtTranslator);


    return a.exec();
}
