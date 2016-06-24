#ifndef GEFRANSEVEN_MOUDLE_THREAD_H
#define GEFRANSEVEN_MOUDLE_THREAD_H

#include <QObject>
#include <QThread>

class gefranseven_moudle_thread : public QThread
{
    Q_OBJECT
public:
    explicit gefranseven_moudle_thread(QObject *parent);
    QObject *parent;
    bool playflag;
private :
    void run();

signals:

public slots:
};

#endif // GEFRANSEVEN_MOUDLE_THREAD_H
