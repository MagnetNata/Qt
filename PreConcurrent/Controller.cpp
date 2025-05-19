#include "Controller.h"

Controller::Controller(QMutex* mutex, QObject* parent)
    : QObject(parent), m_mutex(mutex)
{
    m_worker = new ExampleRace(m_mutex);
    m_worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &Controller::operate, m_worker, &ExampleRace::DoWork);
    connect(m_worker, &ExampleRace::sig_Finish, this, &Controller::sig_WorkFinish);

    workerThread.start();
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}
