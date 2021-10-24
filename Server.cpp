//
// Created by Виктория Картонис on 24.10.2021.
//

#include <netinet/in.h>
#include <zconf.h>
#include <iostream>
#include <arpa/inet.h>
#include "Server.h"

int Server::acceptWithClient()
{
    auto socket = accept(_socket, nullptr, nullptr);
    if(socket == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    return socket;
}

Server::Server(unsigned short int port, char* address, size_t backLog)
{
    const auto sockDesc = socket(AF_INET, SOCK_STREAM, 9000);
    if(sockDesc == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    const auto sockInetAddress = in_addr{inet_addr(address)};
    const auto sockaddressInternet = sockaddr_in{sizeof(in_addr), AF_INET, port, sockInetAddress, 0};

    const auto bindReturnedError = bind(sockDesc, reinterpret_cast<const sockaddr *>(&sockaddressInternet), sizeof(sockaddr_in));
    if(bindReturnedError == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    const auto listenReturnedError = listen(sockDesc, backLog);
    if(listenReturnedError == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    _socket = sockDesc;
}

int Server::receiveFromClient()
{
    _message = new char[_messageLen];
    int total = 0;
    int actuallyReceivedBytes = 0;

    while(total < _messageLen)
    {
        actuallyReceivedBytes = recv(_socket, _message+total, _messageLen-total, 0);
        if(actuallyReceivedBytes == -1)
        {
            break;
        }
        if(actuallyReceivedBytes == 0)
        {
            close(_socket);
        }
        total += actuallyReceivedBytes;
    }
    return (actuallyReceivedBytes == -1 ? -1 : total);
}

void Server::printMessage()
{
    for(auto i = 0; i < _messageLen; ++i)
    {
        std::cout << _message[i];
    }
}

Server::~Server()
{
    delete[] _message;
}
