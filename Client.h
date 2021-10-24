//
// Created by Виктория Картонис on 24.10.2021.
//

#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H


#include <cstddef>

class Client
{
    int _socket;

public:
    void connectWithServer(char* servAddr);
    Client(){};
    int sendToServer(int socketId, char* message, int len);
    void socketClose();
};


#endif //PROXY_CLIENT_H
