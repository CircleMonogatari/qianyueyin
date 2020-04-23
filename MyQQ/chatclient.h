#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <QString>
#include <QStringList>
#include <QtNetwork>
#include <QTcpSocket>

#include<vector>
#include<list>


class chatClient
{
private:
    QString name;
    QString password;
    QString timeTag;
    int pos;

    chatClient();
    chatClient(const chatClient&);
    chatClient& operator=(const chatClient&);

    static chatClient *instance;

public:
    enum typeData{
        SEND_TYPE_GETUSERLISTS,
        SEND_TYPE_GETCHATDATA,
        SEND_TYPE_LOGIN,
        SEND_TYPE_REGISTER,
        SEND_TYPE_SENDDATA
    };

    static chatClient* getChatClientInstance();

    bool isLogin();
    QString login(QString name, QString password);
    QString autoRegister(QString name, QString password);
    
    //send
    QString sendData(int mode,QString data);
    QString sendChatBody(QString body);
    
    //get
    QStringList getUserList();
    QString getChatData();
    std::list<QString> getdata();
    int readData(QTcpSocket *qc, char *outbuf);
};

#endif // CHATCLIENT_H
