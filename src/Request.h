#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class Request {

    private:
    std::string method;
    std::string path;
    std::string httpVersion;

    public:

    void parse(std::string buffer);
    void line(std::string reqline);
    void logRequest();

    std::string getMethod();
    std::string getpath();
    std::string getHttpVersion();
};

#endif