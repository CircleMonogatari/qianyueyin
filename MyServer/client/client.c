//
// Created by 尚富荣 on 2020/3/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8090
#define SERVER_IP "127.0.0.1"

int main() {

    struct sockaddr_in serv_addr;

    //创建套接字
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        printf("socket init error\n");
        exit(1);
    }
    printf("0\n");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //连接服务器
    if (connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("connect error\n");
        exit(1);
    }
    printf("1\n");

    char *p = (char *) "123456";
    int p_len = htonl(strlen(p));

    char *buf = (char *) malloc(p_len + 4);
    memset(buf, 0, p_len + 4);

    memcpy(buf, &p_len, 4);
    memcpy(buf + 4, p, p_len);
    printf("2\n");


    printf("--%x--\n", *(int *) buf);
    printf("--%s--\n", buf + 4);


    if (send(socketfd, buf, p_len + 4, 0) < 0) {
        printf("send error\n");
        exit(1);
    }

    printf("3\n");


    int a = 0x123456;
    int b = 0;
    b = htonl(a); //56341200 转为小端



    return 0;
}