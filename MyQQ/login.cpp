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

//关闭
void Login::on_pushButton_2_clicked()
{
    exit(0);
}

//登陆
void Login::on_pushButton_clicked()
{
    chatClient* c = chatClient::getChatClientInstance();
    QString name = ui->textEdit_2->toMarkdown();
    QString password = ui->textEdit->toMarkdown();

    if (c->login(name, password)){
        this->reject();
        return;
    }
    QMessageBox::information(this,tr("Error"),QStringLiteral("账号或密码不正确"),QMessageBox::Ok);

}

