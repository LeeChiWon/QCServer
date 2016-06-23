#ifndef ES600_MODBUS_THREAD_H
#define ES600_MODBUS_THREAD_H

#include <QObject>

#include <QThread>
#include <QWaitCondition>
#include <QMutex>



class es600_modbus_thread : public QThread
{
   Q_OBJECT
public:
    explicit es600_modbus_thread(QObject *parent_src);
    bool playflag;
    QObject *parent;
    quint16 reg[10];


private:
    void run();

signals:
    void sig_statue_update(bool statue);

public slots:
};

#endif // ES600_MODBUS_THREAD_H
