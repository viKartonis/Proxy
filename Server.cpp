//
// Created by Виктория Картонис on 24.10.2021.
//

#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <iostream>
#include "Server.h"

int Server::acceptWithClient()
{
    auto socket = accept(_acceptedSocket, nullptr, nullptr);
    errorHandler(socket);
    _communicationSocket = socket;
    return socket;
}

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

int Server::receiveFromClient()
{
    _message = new char[_messageLen];
    int total = 0;
    int actuallyReceivedBytes = 0;

    while (true)
    {
        actuallyReceivedBytes = recv(_communicationSocket, _message, _messageLen, 0);
        if (actuallyReceivedBytes == -1 || actuallyReceivedBytes == 0 || actuallyReceivedBytes < _messageLen)
        {
            break;
        }
        total += actuallyReceivedBytes;
        _allMessage += _message;
    }

    std::cout << _allMessage << " ";
    return (actuallyReceivedBytes == -1 ? -1 : total);
}


Server::~Server()
{
    delete[] _message;
}

int Server::sendToClient()
{
    int total = 0;
    int actuallySentBytes = 0;

    for(auto i = 0; i < _messageLen; ++i)
    {
        _message[i] = 'a';
    }

    while(total < _messageLen)
    {
        actuallySentBytes = send(_communicationSocket, _message+total, _messageLen-total, 0);
        if(actuallySentBytes == -1)
        {
            break;
        }
        total += actuallySentBytes;
    }

    return (actuallySentBytes == -1 ? -1 : total);
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
