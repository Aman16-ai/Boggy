#include "Request.h"
#include <iostream>
#include <string>
#include <sstream>

void Request::line(std::string reqline)
{
    std::istringstream iss(reqline);
    iss >> method >> path >> httpVersion;
}

void Request::logRequest()
{
    std::cout << "method : " << method << "size:" << sizeof(method) << std::endl;
    std::cout << "path : " << path << std::endl;
    std::cout << "httpVersion : " << path << std::endl;
}

void Request::parse(std::string buffer)
{

    std::istringstream iss(buffer);
    std::string requestLine;

    if (!std::getline(iss, requestLine))
    {
        return;
    }

    line(requestLine);
}

std::string Request::getMethod()
{
    return this->method;
}
std::string Request::getpath()
{
    return this->path;
}
std::string Request::getHttpVersion()
{
    return this->httpVersion;
}