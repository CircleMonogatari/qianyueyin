#include "chatclient.h"
#include <iostream>
#include <qDebug>

chatClient::chatClient()
{
    this->name = "";
    this->timeTag = "0";
    this->password = "";
    this->pos = 0;
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
QString chatClient::login(QString name, QString password)
{
    QString data = QString("%1\n%2").arg(name).arg(password);


    QString Res = this->sendData(SEND_TYPE_LOGIN, data);
    if(Res == QString("ok")){
        //TODO: 账号登陆
        this->name = name;
        this->password = password;
    }
    return Res;

}

//注册账号
QString chatClient::autoRegister(QString name, QString password)
{

    QString data = QString("%1\n%2").arg(name).arg(password);

    QString Res = this->sendData(SEND_TYPE_REGISTER, data);
    if(Res == QString("ok")){
        //TODO: 账号登陆
        this->name = name;
        this->password = password;
    }

    return Res;
}



QString chatClient::sendData(int mode, QString Req)
{
    QString Res;
    QTcpSocket* client = new QTcpSocket();
//    client->connectToHost("127.0.0.1", 8090);
    client->connectToHost("39.106.149.207", 8090);
    if(client->waitForConnected()){
        char* p = nullptr;

        std::string data;

        data.append(std::to_string(mode));
        data.append(Req.toStdString().c_str());
        int bodylen = data.size();
        quint32 datalen = qToBigEndian(bodylen);

        p = (char*)malloc(data.size() + 4);
        memset(p, 0, data.size()+4);
        memcpy(p, &datalen, 4);
        memcpy(p+4, data.c_str(), data.size());

        client->write(p, data.size() + 4);
        client->waitForBytesWritten(); //即时写入数据

        free(p);
        p = nullptr;

        if(client->waitForReadyRead()){
            Res.append(client->readAll());
        }

    }
    delete client;
    client = nullptr;
    return Res;
}

static QStringList qls;
//发送聊天消息
QString chatClient::sendChatBody(QString body){
    qDebug() << "发送了消息:" << body.toStdString().c_str() ;
    chatClient::typeData m;
    m = SEND_TYPE_SENDDATA;

    QString data = QString("%1 : \n    %2\n").arg(this->name).arg(body);

    QString Res = this->sendData(m, data);

    return Res;
}


//获取用户列表
QStringList chatClient::getUserList()
{
    QString Req;
    Req.append(this->name);
    QString Res = this->sendData(SEND_TYPE_GETUSERLISTS, Req);


    return Res.split("\n");
//    QStringList ql;
//    ql.append(QString("123"));
//    return ql;
}

//获取聊天内容
//QStringList chatClient::getChatData()
QString chatClient::getChatData()
{
    QString Req;
    Req.append(QString("pos=%1").arg(this->pos));


    QString Res = this->sendData(SEND_TYPE_GETCHATDATA, Req);
    int pos = Res.section("\n", 0, 0).toInt();

    if(this->pos == pos){
        //当没有数据时 返回空
        return QString();
    }

    this->pos = pos;
    return Res.section("\n",1);
}


