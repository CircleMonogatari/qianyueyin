#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <QString>
#include<vector>
#include<list>
#include <QStringList>



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
        REGISTER
    };

    static chatClient* getChatClientInstance();

    bool isLogin();
    bool login(QString name, QString password);
    
    //send
    bool sengData(int mode,QString data);
    bool sendChatBody(QString body);
    
    //get
    QStringList getUserList();
    QStringList getChatData();
    
    
    std::list<QString> getdata();
};

#endif // CHATCLIENT_H
