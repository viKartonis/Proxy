//
// Created by Виктория Картонис on 01.11.2021.
//

#include "HTTP.h"

int HTTP::checkHttp(std::string _request, std::string& host)
{
    auto reqLen = _request.length();
    auto isValidMethod = false;

    for(auto i = 0; i < reqLen; ++i)
    {
        if(_request[i] == ' ' && !isValidMethod)
        {
            auto method = _request.substr(0, i);
            auto httpMethods = getMethods();
            for(const auto & httpMethod : httpMethods)
            {
                if(method == httpMethod)
                {
                    isValidMethod = true;
                    break;
                }
            }
        }

        if(_request[i] == '\n')
        {
            auto begin = i+1;
            for(auto j = begin; j < reqLen; ++j)
            {
                if(_request[j] == ':')
                {
                    auto header = _request.substr(begin, j - begin);
                    if(header == "Host")
                    {
                        auto nameBegin = j+2;
                        while(_request[j] != '\r')
                        {
                            j++;
                        }
                        host = _request.substr(nameBegin, j - nameBegin);///
                    }
                }
            }
        }
    }
    if(!isValidMethod)
    {
        return -1;
    }
    return 0;
}
