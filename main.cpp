#include <iostream>
#include "Server.h"

int main()
{
    char* hostname = "127.0.0.1";
    try
    {
        Server server(8080, hostname, 1024);

        server.acceptWithClient();
        server.receiveFromClient();
        server.sendToClient();
    }
    catch(std::runtime_error& error)
    {
        std::cout << error.what();
    }
    return 0;
}