cmake_minimum_required(VERSION 3.15)
project(Proxy)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proxy main.cpp Server.cpp Server.h Client.cpp Client.h HTTP.cpp HTTP.h)