#ifndef BLUECON_MODBUS_THREAD_H
#define BLUECON_MODBUS_THREAD_H

#include <QObject>

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class bluecon_modbus_thread : public QThread
{
    Q_OBJECT
public:
    explicit bluecon_modbus_thread(QObject *parent_src);
    bool playflag;
    QObject *parent;

private:
    void run();

signals:
    void sig_statue_update(bool statue);

public slots:
};

#endif // BLUECON_MODBUS_THREAD_H
