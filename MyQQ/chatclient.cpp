#include "chatclient.h"
#include <iostream>

chatClient::chatClient()
{
    this->name = "";
    this->timeTag = "0";
    this->password = "";
}

chatClient *chatClient::instance = nullptr;

chatClient *chatClient::getChatClientInstance()
{
    if(chatClient::instance == nullptr){
        chatClient::instance = new chatClient();
    }
    return chatClient::instance;
}

bool chatClient::isLogin()
{
    if((this->name == "") && (this->password == "")){
        return false;
    }
    return true;
}

bool chatClient::login(QString name, QString password)
{
    //获取用户名和密码
    std::cout<< "name: " << name.toStdString() <<std::endl;
    std::cout<< "password: " << name.toStdString() <<std::endl;

    this->name = name;
    this->password = password;

    return true;
}
