#include "Server.hpp"


#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include "Chat.h"

#define bzero(x, len) (memset((x),'\0', (len)), (void)0)
#define DATA_LEN 4096

//声明函数
int get_line(int sock, char *buf, int size);

int read_all(int sock, char *buf, int size);

int chat_data(char *data, int len, char *res_data);

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
        char data[DATA_LEN];
        char res_data[DATA_LEN];
        int data_len = 0;


        //调用accept函数后，会进入阻塞状态
        //accept返回一个套接字的文件描述符，这样服务器端便有两个套接字的文件描述符，
        //serverSocket和client。
        //serverSocket仍然继续在监听状态，client则负责接收和发送数据
        //clientAddr是一个传出参数，accept返回时，传出客户端的地址和端口号
        //client_addr_len是一个传入-传出参数，传入的是调用者提供的缓冲区的clientAddr的长度，以避免缓冲区溢出。
        //传出的是客户端地址结构体的实际长度。
        //出错返回-1
        printf("监听端口: %d\n", port);
        client = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if (client < 0) {
            perror("accept");
            continue;
        }
        printf("等待消息...\n");
        //inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
        //表达式：char *inet_ntoa (struct in_addr);
        printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Port is %d\n", htons(client_addr.sin_port));

        //初始化
        memset(data, 0, 4096);

        //读取数据
        data_len = read_all(client, data, DATA_LEN);
        if (data_len <= 0) {
            data_len = 0;
        }

        //处理数据
        int res_data_n = chat_data(data, data_len, res_data);
        //回发数据
        send(client, res_data, res_data_n, 0);
        printf("\n\n[%s]", res_data);

        //回发数据完毕
        printf((char *) "回发数据完毕\n");

        //关闭socket
        close(client);

    }

    close(serverSocket);
    return 0;
}

int chat_data(char *data, int len, char *res_data) {
    printf("接受到消息:");
    printf("%s", data);
    printf("\n");

    char *p = (char *) "服务器已经收到";

    memcpy(res_data, p, strlen(p));

    printf("准备发送消息:\n");
    printf("1. %s %d\n", p, strlen(p));
    printf("2. %s  %d\n", res_data, strlen(res_data));
    printf("\n");

    return strlen(p);
}

int read_all(int sock, char *data, int size) {
    int n = 0;
    int pos = 0;
    char *p = data;
    do {
        printf("【读到数据了】\n");
        n = 0;
        char buf[64];
        memset(buf, 0, 64);
        n = recv(sock, buf, 64, 0);
        printf("[%s]\n", buf);
        if (n <= 0) {
//            if (!(errno == EAGAIN)) {
            break;
//            }
        }
        pos += n;
        if (pos > size) {
            printf("数据包过大\n");
            return pos;
        }
        printf("[%s]\n", buf);
        memcpy(p, buf, n);
        p = p + n;

    } while (n > 0);
    return pos;
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



