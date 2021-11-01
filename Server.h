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
    int _acceptedSocket;
    int _communicationSocket;
    std::string _data;
    std::string _request;

public:
    Server(unsigned short int port, char* address, size_t backLog);
    int acceptWithClient();
    void errorHandler(int errorCode);
    int receiveFromClient();
    int sendToClient();
    int checkHttp();
    ~Server();
};


#endif //PROXY_SERVER_H
