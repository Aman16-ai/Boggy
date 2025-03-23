#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Request.h"
#include "Response.h"
#include "functional"
#include "Router.h"
class HttpServer
{
private:
    int port;      // Port number for the server
    int server_fd; // Socket file descriptor for the server
    Router router;

public:
    HttpServer(int p);
    void setRouter(Router router);
    void listenServer();
    void handle_client(int client_socket);
    void get(std::string method, std::string path, std::function<void(Request &req, Response &res)>);
};

#endif // HTTP_SERVER_H
