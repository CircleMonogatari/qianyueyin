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
     * 注册用户
     * @param name
     * @param password
     * @return 成功0 失败-1
     */
    int registered_user(string name, string password);

    /**
     * 登陆
     * @param name
     * @param password
     * @return 成功0 失败-1
     */
    int log_in(string name, string password);

    /**
     * 聊天框对话
     * @param name 发言人
     * @param data 发送消息
     */
    void add_chat_data(string name, string data);


private:
    static vector<User> users;
    static list<string> chatdatas;
};


#endif //MYSERVER_CHAT_H
