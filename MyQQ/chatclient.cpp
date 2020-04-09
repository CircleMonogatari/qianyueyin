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

bool chatClient::sendChatBody(QString body, QString name, QString password){
     return true;
}


