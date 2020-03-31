//
// Created by Xmas on 2020/3/21.
//

#include "Chat.h"


int Chat::registered_user(string name, string password) {

    for (vector<User>::iterator it = this->users.begin();  it != this->users.end() ; it++) {
        if (it->name == name){
            return -1;
        }
    }

    //add user
    User u;
    u.name = name;
    u.password = password;
    this->users.push_back(u);

    return 0;
}

int Chat::log_in(string name, string password) {
    for (vector<User>::iterator it = this->users.begin();  it != this->users.end() ; it++) {
        if (it->name == name){
            if (it->password == password){
                return 0;
            }
        }
    }
    return -1;
}

void Chat::add_chat_data(string name, string data) {

}
