#include "widget.h"
#include "login.h"
#include "chatclient.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatClient *c = chatClient::getChatClientInstance();
    //�ж��Ƿ��¼
    
    while (!(c->isLogin())) {
        Login l;
        l.exec();

    }
    
    Widget w;
    w.setWindowIcon(QIcon(":/Image/qq.png"));
    w.show();
    return a.exec();
}
