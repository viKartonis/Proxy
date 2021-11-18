//
// Created by Виктория Картонис on 01.11.2021.
//

#ifndef PROXY_HTTP_H
#define PROXY_HTTP_H

#include <vector>
#include <string>

class HTTP
{
private:
    std::vector<std::string> _methods{std::string("OPTIONS"), std::string("GET"), std::string("HEAD"),
                                      std::string("POST"), std::string("PUT"), std::string("PATCH"),
                                      std::string("DELETE"), std::string("TRACE"), std::string("CONNECT")};
public:
    std::vector<std::string> getMethods()
    {
        return _methods;
    }
    int checkHttp(std::string _request, std::string& host);

};


#endif //PROXY_HTTP_H
