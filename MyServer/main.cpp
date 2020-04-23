#include <iostream>
#include "Server.hpp"
#include <map>

using namespace std;

int main() {

    Server *s = Server::getServerInstance();

    cout << "·þÎñÆ÷Æô¶¯ " <<endl;
    s->run(8090);
    return 0;

}