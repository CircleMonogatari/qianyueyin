#include "chatclient.h"
#include <iostream>


chatClient::chatClient()
{
    this->name = "";
    this->timeTag = "0";
    this->password = "";
}

chatClient *chatClient::instance = nullptr;

//单例模式
chatClient *chatClient::getChatClientInstance()
{
    if(chatClient::instance == nullptr){
        chatClient::instance = new chatClient();
    }
    return chatClient::instance;
}

//判断是否登录成功
bool chatClient::isLogin()
{
    if((this->name == "") && (this->password == "")){
        return false;
    }
    return true;
}

//登录账号, 成功会在chatclient中写入账号信息
bool chatClient::login(QString name, QString password)
{
    //获取用户名和密码
    std::cout<< "name: " << name.toStdString() <<std::endl;
    std::cout<< "password: " << name.toStdString() <<std::endl;

    //TODO: 账号登陆
    this->name = name;
    this->password = password;

    return true;
}

static QStringList qls;
//发送聊天消息
bool chatClient::sendChatBody(QString body){
    std::cout << "发送了消息:" << body.toStdString() <<std::endl;
    qls.append(QString(body));
    return true;
}


//获取用户列表
QStringList chatClient::getUserList()
{
    QStringList ql;
    ql.append(QString("123"));
    return ql;
}

//获取聊天内容
QStringList chatClient::getChatData()
{
//    QStringList ql;
//    ql.append(QString("消息\n"));
    return qls;
}


