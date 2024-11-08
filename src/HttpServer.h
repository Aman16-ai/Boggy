#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

class HttpServer {
private:
    int port;           // Port number for the server
    int server_fd;     // Socket file descriptor for the server

public:
    HttpServer(int p);
    void listenServer();
    void handle_client(int client_socket);
};

#endif // HTTP_SERVER_H
