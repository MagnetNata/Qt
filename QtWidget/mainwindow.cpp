#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->RB_1->setText("Первый вариант");
    ui->RB_2->setText("Второй вариант");

    for(int i=1; i<=10; i++)
    {
        ui->CB_spisok->addItem(QString("Элемент № %1").arg(i));
    }

    ui->PB_but->setText("Растем!");
    ui->PB_but->setCheckable(true);

    ui->PB_but->setStyleSheet(
        "QPushButton:checked { background-color: #4CAF50; color: white; }"
        "QPushButton:!checked { background-color: #f0f0f0; }"
        );

    ui->PB_procent->setMinimum(0);
    ui->PB_procent->setMaximum(10);
    ui->PB_procent->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PB_but_clicked()
{
    int maximum = 10;
    int val = ui->PB_procent->value();
    val+=(maximum*0.1);
    if (val>10)
    {
        val=0;
    }
    ui->PB_procent->setValue(val);
}

