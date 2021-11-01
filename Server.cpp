//
// Created by Виктория Картонис on 24.10.2021.
//

#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <iostream>
#include "Server.h"
#include "HTTP.h"

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

    std::string tempMessage;
    while (true)
    {
        actuallyReceivedBytes = recv(_communicationSocket, _message, _messageLen, 0);
        if (actuallyReceivedBytes == -1 || actuallyReceivedBytes == 0 || actuallyReceivedBytes < _messageLen)
        {
            tempMessage += _message;
            break;
        }
        total += actuallyReceivedBytes;
        tempMessage += _message;
    }
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
    auto checkHttpReturnedValue = checkHttp();
    if(checkHttpReturnedValue == -1)
    {
        std::string errorMessage = "It is not a http";
        close(_acceptedSocket);
        close(_communicationSocket);
        throw std::runtime_error(errorMessage);
    }
    std::cout << tempMessage << " ";
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
    auto body = std::string("Hello, world!");
    auto response = std::string("HTTP/1.0 200 OK\nContent-length: ");
    response += std::to_string(body.length());
    response += "\nContent-Type: text/html; charset=utf-8\n\n";
    response += body;

    while(total < response.length())
    {
        actuallySentBytes = send(_communicationSocket, response.data() + total, response.length()-total, 0);
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

int Server::checkHttp()
{
    auto reqLen = _request.length();
    for(auto i = 0; i < reqLen; ++i)
    {
        if(_request[i] == ' ')
        {
            auto method = _request.substr(0, i);
            HTTP http;
            auto httpMethods = http.getMethods();
            for(const auto & httpMethod : httpMethods)
            {
                if(method == httpMethod)
                {
                    return 0;
                }
            }
        }
    }
    return -1;
}
