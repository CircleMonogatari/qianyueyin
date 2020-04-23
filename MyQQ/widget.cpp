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

//ˢ���û��б�
void Widget::RefreshUsers()
{
    //��ȡ����Ȩ��
    chatClient*c = chatClient::getChatClientInstance();


    QStringList users = c->getUserList();

    //�����û��б�
    int iRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(users.size());
    qDebug()<< users;
    for (int i = 0; i < users.size(); i++) {

        QTableWidgetItem* it = new QTableWidgetItem(users.at(i));
        ui->tableWidget->setItem(iRow, i, it);
    }

}

//ˢ�������
void Widget::RefreshChat()
{
    //��ȡ����Ȩ��
    chatClient*c = chatClient::getChatClientInstance();

    //��ȡ�����¼
    QString ql = c->getChatData();
    if(ql == ""){
        return;
    }

    ui->textEdit_2->append(ql);

}

int i = 0;
//��ʱ���� ����ˢ���������
void Widget::runtask()
{
    if(i%5 == 0){
        this->RefreshUsers();
    }
    this->RefreshChat();
    std::cout << i++ << std::endl;
}


//���Ͱ�ť
void Widget::on_pushButton_clicked()
{
    //��ȡ�������������
    QString body = ui->textEdit->toPlainText();
    
    if(body.size() > 255){
       QMessageBox::information(NULL, "Error", QString::fromLocal8Bit("�������ݹ��� �����"));
    }

    //��������
    chatClient* c = chatClient::getChatClientInstance();

    if(c->sendChatBody(body) == QString("ok")){
        ui->textEdit->clear();
    }
    
}
