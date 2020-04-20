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

//��������
int get_line(int sock, char *buf, int size);

int read_all(int sock, char *buf, int size);

int chat_data(char *data, int len, char *res_data);

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
//����socket�������ص��ļ�������
    int serverSocket;    //���������׽���sockaddr_in�ṹ��������ֱ��ʾ�ͻ��˺ͷ�����
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int client;

    int iDataNum;    //socket������ʧ�ܷ���-1
    //int socket(int domain, int type, int protocol);
    // ��һ��������ʾʹ�õĵ�ַ���ͣ�һ�㶼��ipv4��AF_INET
    //�ڶ���������ʾ�׽������ͣ�tcp���������ӵ��ȶ����ݴ���SOCK_STREAM
    //��������������Ϊ0

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    //�˿ڸ���
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));


    bzero(&server_addr, sizeof(server_addr));    //��ʼ���������˵��׽��֣�����htons��htonl���˿ں͵�ַת�������ֽ���
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);    //ip�����Ǳ���������ip��Ҳ�����ú�INADDR_ANY���棬����0.0.0.0���������е�ַ
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //����bind��accept֮��ĺ����������׽��ֲ���������Ҫǿ��ת����(struct sockaddr *)
    //bind�����������������˵��׽��ֵ��ļ���������
    int ret = ::bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect");
        return 1;
    }

    //���÷������ϵ�socketΪ����״̬

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        return 1;
    }

    while (1) {
        char *data = nullptr;
        char res_data[DATA_LEN];
        int data_len = 0;
        int res_data_n = 0;


        //����accept�����󣬻��������״̬
        //accept����һ���׽��ֵ��ļ��������������������˱��������׽��ֵ��ļ���������
        //serverSocket��client��
        //serverSocket��Ȼ�����ڼ���״̬��client������պͷ�������
        //clientAddr��һ������������accept����ʱ�������ͻ��˵ĵ�ַ�Ͷ˿ں�
        //client_addr_len��һ������-����������������ǵ������ṩ�Ļ�������clientAddr�ĳ��ȣ��Ա��⻺���������
        //�������ǿͻ��˵�ַ�ṹ���ʵ�ʳ��ȡ�
        //������-1
        printf("�����˿�: %d\n", port);
        client = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if (client < 0) {
            perror("accept");
            continue;
        }
        printf("�ȴ���Ϣ...\n");
        //inet_ntoa ip��ַת���������������ֽ���IPת��Ϊ���ʮ����IP
        //���ʽ��char *inet_ntoa (struct in_addr);
        printf("IP is [%s] ", inet_ntoa(client_addr.sin_addr));
        printf("Port is [%d]\n", htons(client_addr.sin_port));



        //��ȡ��ͷ ǰ4���ֽ�
        int buf_len = get_client_buf_size(client);
        if (buf_len <= 0) {
            printf("get data len error\n");
            goto END;
        }

        //���������ڴ�
        data = (char *) malloc(buf_len);
        //��ʼ��
        memset(data, 0, buf_len);

        //��ȡ����
        data_len = read_all(client, data, buf_len);
        if (data_len <= 0) {
            data_len = 0;
        }

        //��������
        res_data_n = chat_data(data, data_len, res_data);
        //�ط�����


        send(client, res_data, res_data_n, 0);
        printf("\n\n[%s]", res_data);

        //�ط��������
        printf((char *) "�ط��������\n");

        //�ͷ��ڴ�
        END:
        if (data != nullptr) {
            free(data);
            data = nullptr;
        }

        //�ر�socket
        close(client);

    }


    close(serverSocket);
    return 0;
}

int get_client_buf_size(int client) {

    int buf_len = 0;
    int n = recv(client, &buf_len, 4, 0);
    if (n != 4) {
        printf("n != 4 is %d\n", n);
        return 0;
    }

//    return ntohl(buf_len);
    return (buf_len);
}

int chat_data(char *data, int len, char *res_data) {
    printf("���ܵ���Ϣ:");
    printf("%s", data);
    printf("\n");

    char *p = (char *) "�������Ѿ��յ�";

    memcpy(res_data, p, strlen(p));

    printf("׼��������Ϣ:\n");
    printf("1. %s %d\n", p, strlen(p));
    printf("2. %s  %d\n", res_data, strlen(res_data));
    printf("\n");

    return strlen(p);
}

int write_all(int sock, char *data, int size) {


    char *buf = (char *) malloc(size + 4);
    int data_len = htonl(size);

    memset(buf, 0, size + 4);
    memcpy(buf, &data_len, 4);
    memcpy(buf + 4, data, size);

    //��������
    int writed = send(sock, buf, size + 4, 0);

    //�ͷ��ڴ�
    if (buf != nullptr) {
        free(buf);
        buf = nullptr;
    }
    return writed;
}

int read_all(int sock, char *data, int size) {
    int n = 0;
    int pos = 0;
    char *p = data;


    while (1) {
        printf("�����������ˡ�\n");
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
            printf("���ݰ�����\n");
            return pos;
        }
        printf("[%s]\n", buf);
        memcpy(p, buf, n);
        p = p + n;

    }
    return pos;
}

int get_line(int sock, char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;

    /*����ֹ����ͳһΪ \n ���з�����׼�� buf ����*/
    while ((i < size - 1) && (c != '\n')) {
        /*һ�ν�����һ���ֽ�*/
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0) {
            /*�յ� \r ����������¸��ֽڣ���Ϊ���з������� \r\n */
            if (c == '\r') {
                /*ʹ�� MSG_PEEK ��־ʹ��һ�ζ�ȡ��Ȼ���Եõ���ζ�ȡ�����ݣ�����Ϊ���մ��ڲ�����*/
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                /*������ǻ��з���������յ�*/
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            /*�浽������*/
            buf[i] = c;
            i++;
        } else
            c = '\n';
    }
    buf[i] = '\0';

    /*���� buf �����С*/
    return (i);
}



