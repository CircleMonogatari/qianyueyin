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




QString chatClient::sengData(int mode, QString data)
{
    QString Request;
    QString Response;
    int RequestLen = 0;

    Request.append(QString(mode));
    Request.append(data);

    std::cout<< Request.toStdString() << std::endl;

    RequestLen = Request.size();
    //组建报文
    char* p = nullptr;
    p = (char*)malloc(RequestLen + 4);
    memset(p, 0, RequestLen + 4);
    memcpy(p, (void*)&RequestLen, 4);
    memcpy(p+4, Request.toStdString().c_str(), RequestLen);

    //发送数据
    QTcpSocket *qc;
    qc = new QTcpSocket();
    //链接服务器
    qc->connectToHost(QHostAddress("39.106.149.207"), 8090);
    int a = qc->write(p, RequestLen+4);
//    qc->writeData(p, RequestLen + 4);


    //获取回发信息
    char* res = nullptr;
    int dataLen = 0;
    int ret = 0;
    //读取包头
    ret = qc->read((char*)&dataLen, 4);
    if(ret > 0){
        //获取数据长度
        res = (char*)malloc(dataLen);

        //读取数据
        ret = qc->read(res, dataLen);
        if(ret <= 0){
            printf("读取数据失败\n");
            dataLen = 0;
        }
      Response.append(res);
    }

    //释放内存
    if(nullptr != res){
        free(res);
        res = nullptr;
    }

    if(nullptr != p){
        free(p);
        p = nullptr;
    }


    qc->close();
    //释放句柄
    delete qc;

    return Response;
}

static QStringList qls;
//发送聊天消息
bool chatClient::sendChatBody(QString body){
    std::cout << "发送了消息:" << body.toStdString() <<std::endl;
    chatClient::typeData m;
    m = SENDDATA;
    this->sengData(m, body);

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


