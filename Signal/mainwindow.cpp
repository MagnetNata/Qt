#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_stopwatch(new Stopwatch(this))
{
    ui->setupUi(this);

    ui->PB_Start->setText("Старт");
    ui->PB_Krug->setEnabled(false);
    ui->L_time->setText("0.0");

    connect(ui->PB_Start, &QPushButton::clicked, this, &MainWindow::handleStartStop);
    connect(ui->PB_Clear, &QPushButton::clicked, this, &MainWindow::handleClear);
    connect(ui->PB_Krug, &QPushButton::clicked, this, &MainWindow::handleLap);
    connect(m_stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleStartStop()
{
    m_stopwatch->startStop();

    if (m_stopwatch->isRunning()) {
        ui->PB_Start->setText("Стоп");
        ui->PB_Krug->setEnabled(true);
    } else {
        ui->PB_Start->setText("Старт");
        ui->PB_Krug->setEnabled(false);
    }
}

void MainWindow::handleLap()
{
    Info = m_stopwatch->lap();
    ui->TB_Krug->append(Info);
}

void MainWindow::handleClear()
{
    m_stopwatch->reset();
    ui->L_time->setText("0.0");
    ui->TB_Krug->clear();
    ui->PB_Start->setText("Старт");
    ui->PB_Krug->setEnabled(false);
}

void MainWindow::updateDisplay()
{
    ui->L_time->setText(m_stopwatch->getTime());
}
