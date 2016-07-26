#include "gefransetup_popup.h"
#include "ui_gefransetup_popup.h"
#include "QDebug"

gefransetup_popup::gefransetup_popup(QString machine_name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gefransetup_popup)
{
    ui->setupUi(this);
    machinename = machine_name;
    remotedb = QSqlDatabase::database("remotedb");
    QSqlQuery remotequery(remotedb);
    remotequery.exec(QString("select option_data from Systeminfo where machine_name = '%1'").arg(machinename));
    if(remotequery.next()){
        QString option_data = remotequery.value("option_data").toString();
        QStringList option_data_list = option_data.split(',');

        //size가 1이면 기존에 update가 된것이 없기 때문에 기본 으로 해줌.
        if(option_data_list.size()==1){
            default_option_insert();
        }else {
            for(int i=0;i<option_data_list.size()-1;i++){
                QString list_data = option_data_list.at(i);
                QString list_valuename = list_data.split('=').at(0);
                QString list_valueresult = list_data.split('=').at(1);
                if(list_valuename.compare("hoperuse")==0){
                    ui->hoperusecombox->setCurrentText(list_valueresult);
                }
            }
        }
    }
}

gefransetup_popup::~gefransetup_popup()
{
    delete ui;
}

void gefransetup_popup::on_buttonBox_accepted()
{
    QSqlQuery remotequery(remotedb);
    QString update_str = QString("update Systeminfo set option_data = '");
    update_str.append(QString("hoperuse=%1,").arg(ui->hoperusecombox->currentText()));
    update_str.append(QString("' where machine_name = '%1'").arg(machinename));
    remotequery.exec(update_str);

}
void gefransetup_popup::default_option_insert(){
    QSqlQuery remotequery(remotedb);
    remotequery.exec(QString("update Systeminfo set option_data = 'hoperuse=YES,'  where machine_name = '%1'").arg(machinename));
}
