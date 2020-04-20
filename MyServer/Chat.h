//
// Created by Xmas on 2020/3/21.
//

#ifndef MYSERVER_CHAT_H

#include <vector>
#include <string>
#include <list>

#define MYSERVER_CHAT_H
using namespace std;

typedef struct chat_data {
    int type;
    string data;
} ChatData;

typedef struct user {
    string password;
    string name;
} User;

class Chat {

public:
    /**
     * ע���û�
     * @param name
     * @param password
     * @return �ɹ�0 ʧ��-1
     */
    int registered_user(string name, string password);

    /**
     * ��½
     * @param name
     * @param password
     * @return �ɹ�0 ʧ��-1
     */
    int log_in(string name, string password);

    /**
     * �����Ի�
     * @param name ������
     * @param data ������Ϣ
     */
    void add_chat_data(string name, string data);


public:
    vector<User> users;
    list<string> chatdatas;

};


#endif //MYSERVER_CHAT_H
