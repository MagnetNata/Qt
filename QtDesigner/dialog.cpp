#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString host = ui->LE_Host->text();
    qDebug() << "Хост:" << host;

    QString db = ui->LE_DB->text();
    qDebug() << "База данных:" << db;

    QString login = ui->LE_Login->text();
    qDebug() << "Логин:" << login;

    QString pass = ui->LE_Pass->text();
    qDebug() << "Пароль:" << pass;

    uint port = ui->SB_Port->value();
    qDebug() << "Порт:" << port;
}

