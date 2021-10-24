//
// Created by Виктория Картонис on 24.10.2021.
//

#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include <vector>

class Server
{
    char* _message;
    int _messageLen = 1024;
    int _socket;

public:
    Server(unsigned short int port, char* address, size_t backLog);
    int acceptWithClient();
    int receiveFromClient();
    void printMessage();
    ~Server();
};


#endif //PROXY_SERVER_H
