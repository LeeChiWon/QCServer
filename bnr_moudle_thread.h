#ifndef BNR_MOUDLE_THREAD_H
#define BNR_MOUDLE_THREAD_H

#include <QObject>
#include <QThread>

class bnr_moudle_thread : public QThread
{
    Q_OBJECT
public:
    explicit bnr_moudle_thread(QObject *parent);
    QObject *parent;
    bool playflag;

private:
    void run();

signals:

public slots:
};

#endif // BNR_MOUDLE_THREAD_H
