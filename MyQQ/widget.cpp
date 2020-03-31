#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPushButton *qbtn = new QPushButton(this);
    qbtn->setText(QString("关闭"));
    connect(qbtn, &QPushButton::clicked, this, &Widget::close);

}

Widget::~Widget()
{
    delete ui;
}

