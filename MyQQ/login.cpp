#include "login.h"
#include "ui_login.h"
#include "chatclient.h"
#include <QMessageBox>
#include <iostream>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

//¹Ø±Õ
void Login::on_pushButton_2_clicked()
{
    exit(0);
}

//µÇÂ½
void Login::on_pushButton_clicked()
{
    chatClient* c = chatClient::getChatClientInstance();
    QString name = ui->textEdit_2->toPlainText();
    QString password = ui->textEdit->toPlainText();
    QString Res = c->login(name, password);
    if (Res == QString("ok")){
        this->reject();
        return;
    }
    QMessageBox::information(NULL, "Error", QString::fromLocal8Bit(Res.toUtf8()));
}


void Login::on_pushButton_3_clicked()
{
    chatClient* c = chatClient::getChatClientInstance();
    QString name = ui->textEdit_2->toPlainText();
    QString password = ui->textEdit->toPlainText();
    QString Res = c->autoRegister(name, password);
    if (Res == QString("ok")){
        this->reject();
        return;
    }
    QMessageBox::information(NULL, "Error", QString::fromLocal8Bit(Res.toUtf8()));
}
