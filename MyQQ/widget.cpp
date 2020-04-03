#include "widget.h"
#include "ui_widget.h"

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
    //获取列表



    QStandardItem *item = new QStandardItem("item1");
    model->appendRow(item);
    item = new QStandardItem("item2");
    model->appendRow(item);
    ui->listView->setModel(model);
    //获取聊天记录
}


void Widget::on_pushButton_clicked()
{
    //获取输入的聊天内容

    //发送内容
}
