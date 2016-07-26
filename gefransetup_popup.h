#ifndef GEFRANSETUP_POPUP_H
#define GEFRANSETUP_POPUP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class gefransetup_popup;
}

class gefransetup_popup : public QDialog
{
    Q_OBJECT

public:
    explicit gefransetup_popup(QString machine_name,QWidget *parent = 0);
    QSqlDatabase remotedb;
    void default_option_insert();
    QString machinename;
    ~gefransetup_popup();

private slots:
    void on_buttonBox_accepted();



private:
    Ui::gefransetup_popup *ui;
};

#endif // GEFRANSETUP_POPUP_H
