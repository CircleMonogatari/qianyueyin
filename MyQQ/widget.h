#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QList>
#include <QAction>
#include <QStandardItemModel>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void RefreshUsers();
    void RefreshChat();

private:
    Ui::Widget *ui;
    QTimer* time;


public slots:
    void runtask();


private slots:
    void on_pushButton_clicked();
};
#endif // WIDGET_H
