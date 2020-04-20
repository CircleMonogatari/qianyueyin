#include "Server.hpp"


#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include "Chat.h"

#define bzero(x, len) (memset((x),'\0', (len)), (void)0)


//��������
int get_line(int sock, char *buf, int size);

string read_all(int sock);

string chat_data(string &data);

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
    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    //���÷������ϵ�socketΪ����״̬

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        return 1;
    }

    while (1) {

        //����accept�����󣬻��������״̬
        //accept����һ���׽��ֵ��ļ��������������������˱��������׽��ֵ��ļ���������
        //serverSocket��client��
        //serverSocket��Ȼ�����ڼ���״̬��client������պͷ�������
        //clientAddr��һ������������accept����ʱ�������ͻ��˵ĵ�ַ�Ͷ˿ں�
        //client_addr_len��һ������-����������������ǵ������ṩ�Ļ�������clientAddr�ĳ��ȣ��Ա��⻺���������
        //�������ǿͻ��˵�ַ�ṹ���ʵ�ʳ��ȡ�
        //������-1
        printf("�����˿�: %d\n", port);
        printf("�ȴ�����...\n");
        client = accept(serverSocket, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if (client < 0) {
            perror("accept");
            continue;
        }

        //inet_ntoa ip��ַת���������������ֽ���IPת��Ϊ���ʮ����IP
        //���ʽ��char *inet_ntoa (struct in_addr);
        printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Port is %d\n", htons(client_addr.sin_port));

        //��ȡ����
        string data = read_all(client);
        if (data.size() > 0) {
            //��������
            string res_data = chat_data(data);
            //�ط�����
            send(client, res_data.c_str(), res_data.size(), 0);
        }
        //�ط��������
        printf((char *) "�ط��������\n");

        //�ر�socket
        close(client);

    }

    close(serverSocket);
    return 0;
}

string chat_data(string &data) {
    string Response;
    printf("%s", data.c_str());
    printf("\n");

    char *p = (char *) "����������";

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
    printf("��ʼ��������\n");
    n = read_data_len(sock);
    if (n <= 0) {
        return Request;
    }
    printf("�յ����ݰ���%d ���ֽ�\n", data_len);

    //��ȡ����

    char *p = (char *) malloc(data_len);
    n = 0;

    memset(p, 0, data_len);
    n = recv(sock, p, data_len, 0);
    if (n != data_len) {
        printf("���մ���\n");
    }
//        if (n <= 0) {
//          if (!(errno == EAGAIN))
//                break;
//        }

    //copy����
    Request.append(p, data_len);
    printf("�Ѷ�ȡ����[%s]\n", Request.c_str());
    return Request;
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



