#ifndef SERVERSETFORM_H
#define SERVERSETFORM_H

/*
 *
 * 접속서버설정
 */
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
namespace Ui {
class Serversetform;
}

class Serversetform : public QWidget
{
    Q_OBJECT

public:
    explicit Serversetform(QWidget *parent = 0);
    QSqlDatabase litedb;
    ~Serversetform();

private slots:

    void on_applybtn_clicked();


    void on_DBtypecombo_currentIndexChanged(const QString &arg1);

private:
    Ui::Serversetform *ui;
};

#endif // SERVERSETFORM_H
