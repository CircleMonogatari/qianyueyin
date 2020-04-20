#include "widget.h"
#include "ui_widget.h"
#include "chatclient.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    time = new QTimer(this);
    time->start(2000);
    connect(time, SIGNAL(timeout()), this, SLOT(runtask()));
    model = new QStandardItemModel(this);


}

Widget::~Widget()
{
    delete ui;
}

int i = 0;

void Widget::runtask()
{
    std::cout << i++ << std::endl;
    //��ȡ����Ȩ��
    chatClient*c = chatClient::getChatClientInstance();

    //�����û��б�
    QStandardItem *item = new QStandardItem("item1");
    model->appendRow(item);
    item = new QStandardItem("item2");
    model->appendRow(item);
    ui->listView->setModel(model);

    //��ȡ�����¼
    QStringList ql = c->getChatData();
    QStringList::iterator i;
    for (i = ql.begin(); i != ql.end(); ++i) {
        ui->textEdit_2->append(*i);
    }


}


//���Ͱ�ť
void Widget::on_pushButton_clicked()
{
    //��ȡ�������������
    QString body = ui->textEdit->toMarkdown();
    
    //��������
    chatClient* c = chatClient::getChatClientInstance();

    c->sendChatBody(body);
    
}
