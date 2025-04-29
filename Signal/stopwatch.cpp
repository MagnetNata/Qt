#include "stopwatch.h"
#include <qdebug.h>

Stopwatch::Stopwatch(QObject *parent)
    : QObject(parent),
    m_running(false),
    m_totalTime(0),
    m_lapTime(0),
    m_lapCount(1)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Stopwatch::updateTime);
}

void Stopwatch::startStop()
{
    if (m_running) {
        timer->stop();
        m_totalTime += mainTimer.elapsed();
        m_lapTime += lapTimer.elapsed();
    } else {
        mainTimer.start();
        lapTimer.start();
        timer->start(100);
    }
    m_running = !m_running;
    emit timeUpdated();
}

void Stopwatch::reset()
{
    timer->stop();
    m_running = false;
    m_totalTime = 0;
    m_lapTime = 0;
    m_lapCount = 1;
    emit timeUpdated();
}

void Stopwatch::lap()
{
    if (!m_running) return;

    qint64 currentLap = lapTimer.elapsed();
    QString lapInfo = QString("Круг %1, время: %2 сек")
                          .arg(m_lapCount++)
                          .arg(QString::number(currentLap / 1000.0, 'f', 1));

    emit lapAdded(lapInfo);
    lapTimer.restart();
}

QString Stopwatch::getTime() const
{
    qint64 time = m_running ? m_totalTime + mainTimer.elapsed() : m_totalTime;
    return QString::number(time / 1000.0, 'f', 1) + " сек";
}

QString Stopwatch::getLapTime() const
{
    qint64 time = m_running ? m_lapTime + lapTimer.elapsed() : m_lapTime;
    return QString("%1.%2")
        .arg(time / 1000)
        .arg((time % 1000) / 100, 1, 10, QChar('0'));
}

bool Stopwatch::isRunning() const
{
    return m_running;
}

void Stopwatch::updateTime()
{
    emit timeUpdated();
}
