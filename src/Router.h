#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <map>
#include "Request.h"
#include "Response.h"
class Router
{

public:
    void get(std::string method, std::string path, std::function<void(Request &req, Response &res)> handler);

    void execute(std::string method, std::string path, Request &req, Response &res);

private:
    /* method -> {path -> handler} */
    std::map<std::string, std::map<std::string, std::function<void(Request &req, Response &res)>>> routes;
};

#endif