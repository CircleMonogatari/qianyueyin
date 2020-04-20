#include <iostream>
#include "Server.hpp"

using namespace std;

int main() {

    Server *s = Server::getServerInstance();

    cout << "服务器启动 " <<endl;
    s->run(8090);
    return 0;

}