#include "ExampleRace.h"

ExampleRace::ExampleRace(QMutex* mutex, QObject* parent)
    : QObject(parent), mut(mutex) {}

void ExampleRace::DoWork(uint32_t* num, bool mutexOn, int numIterat)
{
    if(mutexOn) {
        for(uint32_t i = 0; i < numIterat; i++) {
            mut->lock();
            (*num)++;
            mut->unlock();
        }
    } else {
        for(uint32_t i = 0; i < numIterat; i++) {
            (*num)++;
        }
    }
    emit sig_Finish();
}
