//
// Created by Xmas on 2020/3/21.
//

#include "Chat.h"


Chat *Chat::instance = nullptr;
vector<User> Chat::users;
map<string, int> Chat::usersOl;
vector<string> Chat::chatdatas;


/**
 * ��½�˺�
 * @param �û��� name
 * @param ���� password
 * @return ��ȷ ok   ʧ�� ����ԭ��
 */
string Chat::chat_login(string name, string password) {
    for (vector<User>::iterator it = users.begin();  it != users.end() ; it++) {
        if (it->name == name){
            if (it->password == password){
                return string("ok");
            }
        }
    }

    return string("�˺Ż����벻��ȷ");
}

/**
 * ��ȡ�����û��б�
 * @param data
 * @return
 */
string Chat::chat_get_userlist(string &data) {
    string Res;
    time_t t;
    t = time(NULL);
    int des = time(&t);
    //ˢ��ʱ��
    this->usersOl[data] =  des;

    map<string, int>::iterator it = this->usersOl.begin();
    map<string, int>::iterator end = this->usersOl.end();

    int num = 0;
    while(it != end){

        if(des - it->second > 60){
            it++;
            continue;
        }

        std::ostringstream s;
        s << it->first << "\n";
        Res.append(s.str());
        it++;
        num++;
    }
    printf("---��ǰ��������:%d\n", num);
    return Res;
}



string Chat::chat_get_data(string &data) {
    string Res;

    int pos = stoi(data);


    std::ostringstream s;
    s << this->chatdatas.size() << "\n";

    Res.append(s.str());
    for(;pos < this->chatdatas.size(); pos++){
        Res.append(this->chatdatas.at(pos));
    }
    return Res;
}

string Chat::chat_set_data(string &data) {

    this->chatdatas.push_back(data);
    return string("ok");
}

string Chat::chat_register(string name, string password) {
    for (vector<User>::iterator it = users.begin();  it != users.end() ; it++) {
        if (it->name == name){
            return  string("���û��Ѵ���");
        }
    }

    //add user
    User u;
    u.name = name;
    u.password = password;
    users.push_back(u);

    return  string("ok");
}

Chat *Chat::getChatInstance() {
    if (Chat::instance == nullptr){
        Chat::instance = new Chat();
    }

    return Chat::instance;
}

Chat::Chat() {

}

Chat::~Chat() {

}


Chat::Chat(const Chat &) {

}
