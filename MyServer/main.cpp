#include <iostream>
#include "Server.hpp"

using namespace std;

int main() {

    Server *s = Server::getServerInstance();

    s->run(8090);
    return 0;

}