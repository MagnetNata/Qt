#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFutureWatcher>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

    race1 = new Controller(&m);
    race2 = new Controller(&m);
    concurRace1 = new ExampleRace(&m);
    concurRace2 = new ExampleRace(&m);

    auto handleFinish = [this](){
        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) +
                                 ", а должно быть " + QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }
    };

    connect(race1, &Controller::sig_WorkFinish, this, handleFinish);
    connect(race2, &Controller::sig_WorkFinish, this, handleFinish);
    connect(concurRace1, &ExampleRace::sig_Finish, this, handleFinish);
    connect(concurRace2, &ExampleRace::sig_Finish, this, handleFinish);
}

MainWindow::~MainWindow()
{
    delete race1;
    delete race2;
    delete concurRace1;
    delete concurRace2;
    delete ui;
}

void MainWindow::StartRace()
{
    ui->pb_start->setEnabled(false);
    countFinish = 0;
    number = 0;

    if(ui->rb_qtConcur->isChecked()){
        bool useMutex = ui->rb_mutexOn->isChecked();
        int iterations = ui->sb_initNum->value();

        // Используем атомарные операции для точного подсчета
        std::atomic<uint32_t> atomicNumber(0);

        auto worker = [&atomicNumber, iterations, useMutex, this](){
            for(int i = 0; i < iterations; i++){
                if(useMutex){
                    QMutexLocker locker(&m);
                    atomicNumber++;
                } else {
                    atomicNumber++;
                }
            }
        };

        QFuture<void> future1 = QtConcurrent::run(worker);
        QFuture<void> future2 = QtConcurrent::run(worker);

        QFutureWatcher<void> watcher;
        watcher.setFuture(future1);
        watcher.waitForFinished();
        watcher.setFuture(future2);
        watcher.waitForFinished();

        // После завершения обоих потоков сохраняем результат
        number = atomicNumber.load();

        ui->te_debug->append("Искомое число равно: " + QString::number(number) +
                             ", а должно быть " + QString::number(ui->sb_initNum->value()*2));
        ui->pb_start->setEnabled(true);
    }
    else {
        race1->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
        race2->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
    }
}

void MainWindow::on_pb_start_clicked()
{
    StartRace();
}
