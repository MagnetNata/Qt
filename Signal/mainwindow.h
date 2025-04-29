#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleStartStop();
    void handleClear();
    void handleLap();
    void updateDisplay();
    void addLap(QString lapInfo);

private:
    Ui::MainWindow *ui;
    Stopwatch *m_stopwatch;
};
#endif // MAINWINDOW_H
