#ifndef EXAMPLERACE_H
#define EXAMPLERACE_H

#include <QObject>
#include <QMutex>

class ExampleRace : public QObject
{
    Q_OBJECT
public:
    ExampleRace(QMutex* mutex, QObject* parent = nullptr);
    void DoWork(uint32_t* num, bool mutexOn, int numIterat);

signals:
    void sig_Finish();

private:
    QMutex* mut;
};

#endif // EXAMPLERACE_H
