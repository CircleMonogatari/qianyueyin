#include "widget.h"
#include "ui_widget.h"
#include "chatclient.h"
#include <QTableWidget>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    time = new QTimer(this);
    time->start(4000);
    connect(time, SIGNAL(timeout()), this, SLOT(runtask()));

}

Widget::~Widget()
{
    delete ui;
}

//刷新用户列表
void Widget::RefreshUsers()
{
    //获取控制权柄
    chatClient*c = chatClient::getChatClientInstance();


    QStringList users = c->getUserList();

    //加载用户列表
    int iRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(users.size());
    qDebug()<< users;
    for (int i = 0; i < users.size(); i++) {

        QTableWidgetItem* it = new QTableWidgetItem(users.at(i));
        ui->tableWidget->setItem(iRow, i, it);
    }

}

//刷新聊天框
void Widget::RefreshChat()
{
    //获取控制权柄
    chatClient*c = chatClient::getChatClientInstance();

    //获取聊天记录
    QString ql = c->getChatData();
    if(ql == ""){
        return;
    }

    ui->textEdit_2->append(ql);

}

int i = 0;
//定时任务 用于刷新聊天界面
void Widget::runtask()
{
    if(i%5 == 0){
        this->RefreshUsers();
    }
    this->RefreshChat();
    std::cout << i++ << std::endl;
}


//发送按钮
void Widget::on_pushButton_clicked()
{
    //获取输入的聊天内容
    QString body = ui->textEdit->toPlainText();
    
    if(body.size() > 255){
       QMessageBox::information(NULL, "Error", QString::fromLocal8Bit("发送内容过大， 请调整"));
    }

    //发送内容
    chatClient* c = chatClient::getChatClientInstance();

    if(c->sendChatBody(body) == QString("ok")){
        ui->textEdit->clear();
    }
    
}
