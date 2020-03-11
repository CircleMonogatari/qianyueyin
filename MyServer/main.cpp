#include <iostream>
#include "Server.hpp"

using namespace std;

int main() {

    auto s = Server::getServerInstance();

    cout << "123 " <<endl;
    return 0;

}