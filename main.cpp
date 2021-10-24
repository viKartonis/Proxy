#include <ares_build.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "Server.h"
#include "Client.h"

int main()
{
    char* hostname = "127.0.0.1";
    try
    {
        Server server(0, hostname, 1024);
        Client client;
        client.connectWithServer(hostname);

        const auto socketId = server.acceptWithClient();

        char *message = "Hello, world!";
        client.sendToServer(socketId, message, strlen(message));

        auto isClose = 0;
        while (!isClose)
        {
            isClose = server.receiveFromClient();
        }
    }
    catch(std::runtime_error& error)
    {
        std::cout << error.what();
    }
    return 0;
}