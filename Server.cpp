//
// Created by Виктория Картонис on 24.10.2021.
//

#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Server.h"
#include "HTTP.h"

Server::Server(unsigned short int port, char* address, size_t backLog)
{
    const auto sockDesc = socket(AF_INET, SOCK_STREAM, 0);
    errorHandler(sockDesc);

    sockaddr_in sockaddressInternet{};
    memset(&sockaddressInternet, 0, sizeof(sockaddressInternet));
    sockaddressInternet.sin_family = AF_INET;
    sockaddressInternet.sin_addr.s_addr = inet_addr(address);
    sockaddressInternet.sin_port = htons(port);

    const auto bindReturnedError = bind(sockDesc, reinterpret_cast<const sockaddr *>(&sockaddressInternet), sizeof(sockaddressInternet));
    errorHandler(bindReturnedError);
    const auto listenReturnedError = listen(sockDesc, backLog);
    errorHandler(listenReturnedError);
    _acceptedSocket = sockDesc;
}

int Server::acceptWithClient()
{
    auto socket = accept(_acceptedSocket, nullptr, nullptr);
    errorHandler(socket);
    _clientProxySocket = socket;
    return socket;
}

int Server::proxyReceive(std::string& tempMessage, int socket)
{
    _message = new char[_messageLen];
    int total = 0;
    int actuallyReceivedBytes = 0;

    while (true)
    {
        actuallyReceivedBytes = recv(socket, _message, _messageLen, 0);
        if (actuallyReceivedBytes == -1 || actuallyReceivedBytes == 0 || actuallyReceivedBytes < _messageLen)
        {
            tempMessage += _message;
            break;
        }
        total += actuallyReceivedBytes;
        tempMessage += _message;
    }
    return (actuallyReceivedBytes == -1 ? -1 : total);
}

int Server::proxySend(const std::string& response, int socket)
{
    int total = 0;
    int actuallySentBytes = 0;

    while(total < response.length())
    {
        actuallySentBytes = send(socket, response.data() + total, response.length() - total, 0);
        if(actuallySentBytes == -1)
        {
            break;
        }
        total += actuallySentBytes;
    }

    return (actuallySentBytes == -1 ? -1 : total);
}

int Server::toServer()
{
    std::string tempMessage;
    proxyReceive(tempMessage,_clientProxySocket);
    auto tempMessageLen = tempMessage.length();
    for(auto i = 0; i < tempMessageLen; ++i)
    {
        if(i + 3 < tempMessageLen && tempMessage[i] == '\r' && tempMessage[i+1] == '\n' && tempMessage[i+2] == '\r'
        && tempMessage[i+3] == '\n')
        {
            _request = tempMessage.substr(0, i+4);
            _data = tempMessage.substr(i+4, tempMessageLen - (i+4));
        }
    }
    std::cout << tempMessage;
    HTTP http;
    auto checkHttpReturnedValue = http.checkHttp(_request, _host);
    if(checkHttpReturnedValue == -1)
    {
        std::string errorMessage = "It is not a http";
        close(_acceptedSocket);
        close(_clientProxySocket);
        throw std::runtime_error(errorMessage);
    }
    lookUpHost();

//    _serverProxySocket = socket(_realServer->ai_family, _realServer->ai_socktype, _realServer->ai_protocol);
//    errorHandler(_serverProxySocket);
//    auto connectToServerError = connect(_serverProxySocket,_realServer->ai_addr, _realServer->ai_addrlen);
//    errorHandler(connectToServerError);
//    proxySend(_message, _serverProxySocket);

    std::cout << tempMessage << " ";
    return 0;
}


std::string Server::fromServer()
{
    std::string receivedMessage;
    proxyReceive(receivedMessage,_serverProxySocket);
    proxySend(receivedMessage, _clientProxySocket);
    return receivedMessage;
}

int Server::lookUpHost()
{
    auto hostNameSize = _host.size();
    auto postPos = 0;
    for(auto i = 0; i < hostNameSize; ++i)
    {
        if(_host[i] == ':')
        {
            postPos = i;
            break;
        }
    }
    std::string onlyHost;
    std::string onlyPort;
    if(postPos == 0)
    {
        onlyHost = _host;

    }
    else
    {
        onlyHost = _host.substr(0, postPos);
        onlyPort = _host.substr(postPos+1, hostNameSize - postPos - 1);
    }

    struct addrinfo hints{}, *result, *res;
    int errcode;
    _realServerAddress = new char[100];
    void *ptr;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;
    if(postPos == 0)
    {
        errcode = getaddrinfo(onlyHost.c_str(), nullptr, &hints, &result);
    }
    else
    {
        errcode = getaddrinfo(onlyHost.c_str(), onlyPort.c_str(), &hints, &result);
    }
    if (errcode != 0)
    {
        perror ("getaddrinfo");
        return -1;
    }

    res = result;

    while (res)
    {
        _serverProxySocket = socket(AF_INET, SOCK_STREAM, 0);
        errorHandler(_serverProxySocket);
        auto connectToServerError = connect(_serverProxySocket, res->ai_addr, res->ai_addrlen);
        errorHandler(connectToServerError);
        proxySend(_message, _serverProxySocket);
        res = res->ai_next;
    }

    freeaddrinfo(result);
    return 0;
}

void Server::errorHandler(int errorCode)
{
    if(errorCode == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        close(_acceptedSocket);
        throw std::runtime_error(errorMessage);
    }
}

Server::~Server()
{
    delete[] _message;
    delete[] _realServerAddress;
}

