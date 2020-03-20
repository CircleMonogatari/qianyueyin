#include <iostream>
#include "Server.hpp"

using namespace std;

int main() {

    Server* s = Server::getServerInstance();

    cout << "123 " <<endl;
    s->run(8090);
    return 0;

}