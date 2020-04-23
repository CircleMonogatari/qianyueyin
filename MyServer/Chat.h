//
// Created by Xmas on 2020/3/21.
//

#ifndef MYSERVER_CHAT_H

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#define MYSERVER_CHAT_H
using namespace std;

typedef struct user {
    string password;
    string name;
} User;

class Chat {
private:
    Chat();
    ~Chat();

    Chat&operator=(const Chat&);
    Chat(const Chat&);

public:

    static Chat* getChatInstance();

    /**
     * ��½
     * @param data
     * @return
     */
    static string chat_login(string name, string password);

    /**
     * ע��
     * @param data
     * @return
     */
    string chat_register(string name, string password);

    /**
     * ��ȡ�û��б�
     * @param data
     * @return
     */
    string chat_get_userlist(string &data);

    /**
     * ��ȡ�����¼
     * @param data
     * @return
     */
    string chat_get_data(string &data);

    /**
     * ������Ϣ
     * @param data
     * @return
     */
    string chat_set_data(string &data);

public:
    static vector<User> users;
    static map<string, int> usersOl;
    static vector<string> chatdatas;
    static Chat* instance;

};


#endif //MYSERVER_CHAT_H
