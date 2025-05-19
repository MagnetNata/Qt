#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "ExampleRace.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QMutex* mutex, QObject* parent = nullptr);
    ~Controller();

signals:
    void operate(uint32_t* num, bool mutexOn, int numIterat);
    void sig_WorkFinish();

private:
    QThread workerThread;
    QMutex* m_mutex;
    ExampleRace* m_worker;
};

#endif // CONTROLLER_H
