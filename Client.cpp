//
// Created by Виктория Картонис on 24.10.2021.
//

#include <netinet/in.h>
#include <zconf.h>
#include <stdexcept>
#include <arpa/inet.h>
#include "Client.h"

void Client::connectWithServer(char* servAddr)
{
    auto address = inet_addr(servAddr);
    const auto sockDesc = socket(AF_INET, SOCK_STREAM, 0);
    if(sockDesc == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    const auto connectionReturnedError = connect(sockDesc, (sockaddr *)&address, sizeof(address));

    if(connectionReturnedError == -1)
    {
        char errorMessage[128];
        perror(errorMessage);
        throw std::runtime_error(errorMessage);
    }
}

int Client::sendToServer(int socketId, char *message, int len)
{
    int total = 0;
    int actuallySentBytes = 0;

    while(total < len)
    {
        actuallySentBytes = send(socketId, message+total, len-total, 0);
        if(actuallySentBytes == -1)
        {
            break;
        }
        total += actuallySentBytes;
    }

    return (actuallySentBytes == -1 ? -1 : total);
}

void Client::socketClose()
{
    close(_socket);
}
