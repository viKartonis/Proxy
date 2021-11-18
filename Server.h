//
// Created by Виктория Картонис on 24.10.2021.
//

#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include <vector>

class Server
{
    char* _message;
    char* _realServerAddress;
    struct addrinfo* _realServer;
    int _messageLen = 1024;
    int _acceptedSocket;
    int _clientProxySocket;
    int _serverProxySocket;
    std::string _data;
    std::string _request;
    std::string _host;
    void errorHandler(int errorCode);
    int lookUpHost();

public:
    Server(unsigned short int port, char* address, size_t backLog);
    int acceptWithClient();
    int toServer();
    std::string fromServer();
    int proxyReceive(std::string& tempMessage, int socket);
    int proxySend(const std::string& response, int socket);
    ~Server();
};


#endif //PROXY_SERVER_H
