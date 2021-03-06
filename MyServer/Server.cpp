#include "Server.hpp"

#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include "Chat.h"

#include <string>
#include <vector>

#define bzero(x, len) (memset((x),'\0', (len)), (void)0)



enum typeData{
    SEND_TYPE_GETUSERLISTS,
    SEND_TYPE_GETCHATDATA,
    SEND_TYPE_LOGIN,
    SEND_TYPE_REGISTER,
    SEND_TYPE_SENDDATA
};

//声明函数
int get_line(int sock, char *buf, int size);

string read_all(int sock);

string chat_data(string &data);

int get_client_buf_size(int client);

Server::Server(/* args */) {

}

Server::~Server() {

}

Server::Server(const Server &) {

}

Server *Server::instance = nullptr;

Server *Server::getServerInstance() {
    if (instance == nullptr) {
        instance = new Server();
    }
    return instance;
}

int Server::run(int port) {
//调用socket函数返回的文件描述符
    int serverSocket;    //声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int client;

    int iDataNum;    //socket函数，失败返回-1
    //int socket(int domain, int type, int protocol);
    // 第一个参数表示使用的地址类型，一般都是ipv4，AF_INET
    //第二个参数表示套接字类型：tcp：面向连接的稳定数据传输SOCK_STREAM
    //第三个参数设置为0

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    //端口复用
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));


    bzero(&server_addr, sizeof(server_addr));    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);    //ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
    //bind三个参数：服务器端的套接字的文件描述符，
    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    //设置服务器上的socket为监听状态

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        return 1;
    }

    while (1) {

        //调用accept函数后，会进入阻塞状态
        //accept返回一个套接字的文件描述符，这样服务器端便有两个套接字的文件描述符，
        //serverSocket和client。
        //serverSocket仍然继续在监听状态，client则负责接收和发送数据
        //clientAddr是一个传出参数，accept返回时，传出客户端的地址和端口号
        //client_addr_len是一个传入-传出参数，传入的是调用者提供的缓冲区的clientAddr的长度，以避免缓冲区溢出。
        //传出的是客户端地址结构体的实际长度。
        //出错返回-1
        printf("监听端口: %d\n", port);
        printf("等待连接...\n");
        client = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if (client < 0) {
            perror("accept");
            continue;
        }

        //inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
        //表达式：char *inet_ntoa (struct in_addr);
        printf("============接到链接=================\n");
        printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Port is %d\n", htons(client_addr.sin_port));

        //读取数据
        string data = read_all(client);
        if (data.size() > 0) {
            //处理数据
            string res_data = chat_data(data);
            //回发数据
            printf("----回发数据：%s\n", res_data.c_str());
            send(client, res_data.c_str(), res_data.size(), 0);
        }
        //回发数据完毕
        printf((char *) "--------------回发数据完毕-------------\n");

        //关闭socket
        close(client);

    }

    close(serverSocket);
    return 0;
}

void split(const string&s, vector<string>& tokens, const string& delimiters= " "){

    //找到delimiters 位置
    int pos = s.find(delimiters);
    if(pos > 0){
        string user = string(s.c_str(), pos); //字符串是拷贝多少个
        string password = string(s, pos+1);       //string 是从第几个开始
        tokens.push_back(user);
        tokens.push_back(password);
    }

}



string chat_data(string &data) {
    string Response;
    string mode(data.c_str(), 1);
    string p(data, 1);
    printf("%s", p.c_str());
    printf("mode is %d", std::stoi(mode));
    printf("\n");
    Chat* c = Chat::getChatInstance();
    vector<string> d;

    printf("开始处理：\n --mode= %s--\n", mode.c_str());
    printf("--%s--\n", p);

    switch(std::stoi(mode)){
        case SEND_TYPE_GETCHATDATA:
            split(p, d, "=");
            Response.append(c->chat_get_data(d.at(1)));
            break;
        case SEND_TYPE_GETUSERLISTS:
            Response.append(c->chat_get_userlist(p));
            break;
        case SEND_TYPE_LOGIN:

            split(p, d, "\n");
            if(d.size() != 2){
                Response.append(string("参数不正确"));
            }
            Response.append(c->chat_login(d.at(0), d.at(1)));
            break;

        case SEND_TYPE_REGISTER:

            split(p, d, "\n");
            if(d.size() != 2){
                Response.append(string("参数不正确"));
            }
            Response.append(c->chat_register(d.at(0), d.at(1)));
            break;
        case SEND_TYPE_SENDDATA:
            Response.append(c->chat_set_data(p));
            break;
    }

    return Response;
}

int read_data_len(int sock) {
    int n = 0;

    recv(sock, &n, 4, 0);
    n = ntohl(n);
    return n;
}

string read_all(int sock) {
    string Request;
    int data_len = 0;
    int n = 0;
    printf("开始接收数据\n");
    data_len = read_data_len(sock);
    if (data_len <= 0) {
        return Request;
    }

    if (data_len > 4096 * 4){
        printf("数据过大 不合理\n");
        return Request;
    }

    printf("收到数据包：%d 个字节\n", data_len);

    //读取数据

    char *p = (char *) malloc(data_len+1);
    n = 0;

    memset(p, 0, data_len+1);
    n = recv(sock, p, data_len, 0);
    if (n != data_len) {
        printf("接收错误\n");
    }
//        if (n <= 0) {
//          if (!(errno == EAGAIN))
//                break;
//        }

    //copy内容
    Request.append(p, data_len);
    free(p);
    printf("读取完毕\n");
    return Request;
}

int get_line(int sock, char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;

    /*把终止条件统一为 \n 换行符，标准化 buf 数组*/
    while ((i < size - 1) && (c != '\n')) {
        /*一次仅接收一个字节*/
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0) {
            /*收到 \r 则继续接收下个字节，因为换行符可能是 \r\n */
            if (c == '\r') {
                /*使用 MSG_PEEK 标志使下一次读取依然可以得到这次读取的内容，可认为接收窗口不滑动*/
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                /*但如果是换行符则把它吸收掉*/
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            /*存到缓冲区*/
            buf[i] = c;
            i++;
        } else
            c = '\n';
    }
    buf[i] = '\0';

    /*返回 buf 数组大小*/
    return (i);
}

