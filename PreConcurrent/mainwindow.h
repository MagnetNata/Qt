#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QtConcurrent>
#include "ExampleRace.h"
#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override; // Явное объявление деструктора

private slots:
    void on_pb_start_clicked();

private:
    Ui::MainWindow *ui;
    uint32_t number = 0;
    uint32_t countFinish = 0;
    QMutex m;

    Controller *race1 = nullptr;
    Controller *race2 = nullptr;
    ExampleRace *concurRace1 = nullptr;
    ExampleRace *concurRace2 = nullptr;

    void StartRace();
};
#endif // MAINWINDOW_H
