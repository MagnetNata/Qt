#include <QCoreApplication>
#include <QTcpSocket>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket socket;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    qDebug() << "Программа успешно создала:";
    qDebug() << "1. Объект QTcpSocket:" << &socket;
    qDebug() << "2. Объект QSqlDatabase:" << &db;

    return 0;
}
