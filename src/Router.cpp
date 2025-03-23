#include "Router.h"
#include "Request.h"
#include "Response.h"
#include <functional>
#include <map>
#include <iostream>
#include <stdexcept>

void Router::get(std::string method, std::string path, std::function<void(Request &req, Response &res)> handler)
{
    routes[method][path] = handler;
}

void Router::execute(std::string method, std::string path, Request &req, Response &res)
{

    std::cout << "LOG :: Running Execute --------------" << std::endl;
    for (auto m : routes)
    {
        std::cout << "DATA :: " << m.first << std::endl;
    }
    if (routes.find(method) != routes.end())
    {
        std::cout << "LOG :: Running inside method " << method << std::endl;
        if (routes[method].find(path) != routes[method].end())
        {
            std::cout << "LOG :: Running inside method " << method << std::endl;
            routes[method][path](req, res);
        }
    }
    else
        throw std::runtime_error("Error :: Route not find");
}