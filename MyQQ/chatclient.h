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

    chatClient();
    chatClient(const chatClient&);
    chatClient& operator=(const chatClient&);

    static chatClient *instance;

public:
    enum typeData{
        GETUSERLISTS,
        GETCHATDATA,
        LOGIN,
        REGISTER,
        SENDDATA
    };

    static chatClient* getChatClientInstance();

    bool isLogin();
    bool login(QString name, QString password);
    
    //send
    QString sengData(int mode,QString data);
    bool sendChatBody(QString body);
    
    //get
    QStringList getUserList();
    QStringList getChatData();
    std::list<QString> getdata();
    int readData(QTcpSocket *qc, char *outbuf);
};

#endif // CHATCLIENT_H
