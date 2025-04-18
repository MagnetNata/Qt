#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QMessageBox msg;

    msg.setText("Hello");
    msg.setWindowTitle("First");
    msg.setStyleSheet("color: red");
    msg.setIcon(QMessageBox::Information);

    msg.show();

    return a.exec();
}
