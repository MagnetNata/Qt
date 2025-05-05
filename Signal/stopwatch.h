#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    void startStop();
    void reset();
    QString lap();
    QString getTime() const;
    QString getLapTime() const;
    bool isRunning() const;

private:
    QTimer *timer;
    QElapsedTimer mainTimer;
    QElapsedTimer lapTimer;
    bool m_running;
    qint64 m_totalTime;
    qint64 m_lapTime;
    int m_lapCount;

public slots:
    void updateTime();

signals:
    void timeUpdated();
};

#endif // STOPWATCH_H
