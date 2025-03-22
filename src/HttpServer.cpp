#include "HttpServer.h"
#include <fstream>
#include <sstream>
#include "Response.h"
#include "Request.h"
void send_response(int client_socket, const std::string& content, const std::string& content_type) {
    std::ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: " << content_type << "\r\n"
        << "Content-Length: " << content.size() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << content;

    send(client_socket, oss.str().c_str(), oss.str().length(), 0);
}

std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

HttpServer::HttpServer(int p) : port(p), server_fd(-1) {
    // Constructor initializes the port number and server_fd
}

void HttpServer::handle_client(int client_socket) {
    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        close(client_socket);
        return; // Handle error or empty read
    }
    
    buffer[bytes_read] = '\0'; // Null-terminate the string
    std::string request(buffer);

    // write a parse_request method to  parse the request to get the path and portocal call the method of the correspoding path 
    // use map to store this



    /*
        we will run this code on another thread to achieve more concurrency
        
        Request req = parse_request(buffer);
        Response res;
        route[req.path](req,res)
        send(client_socket, res.data.c_str(), res.data.length(), 0);
    
    */

    Request req;
    req.parse(request);
    req.logRequest();


    // Check if it's a GET request
    if (request.substr(0, 3) == "GET") {
        std::cout << "GET request received\n";

        // Extract the requested file path
        size_t pos = request.find(" ");
        std::string file_path = request.substr(pos + 1, request.find(" ", pos + 1) - pos - 1);
        
        if (file_path == "/") {
            file_path = "/index.html";  // Default to index.html
        }
        
        file_path = "." + file_path;  // Relative path
        
        // Read the file content
        std::string content = read_file(file_path);
        std::cout<<"--------------------------"<<std::endl;
        // Determine the content type
        std::string content_type = "text/html";
        if (file_path.find(".css") != std::string::npos) {
            content_type = "text/css";
        } else if (file_path.find(".js") != std::string::npos) {
            content_type = "application/javascript";
        } else if (file_path.find(".png") != std::string::npos) {
            content_type = "image/png";
        } else if (file_path.find(".jpg") != std::string::npos || file_path.find(".jpeg") != std::string::npos) {
            content_type = "image/jpeg";
        }

        // Send the response
        if (!content.empty()) {
            // send_response(client_socket, content, content_type);
            Response res(client_socket);
            res.send_response(content,content_type);
        } else {
            // If the file was not found, send a 404 response
            std::string not_found = "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/html\r\n\r\n"
                                    "<h1>404 Not Found</h1>";
            send(client_socket, not_found.c_str(), not_found.length(), 0);
        }
    }
    // Handle POST request
    else if (request.substr(0, 4) == "POST") {
        std::cout << "POST request received\n";

        // For simplicity, send a success response (handle form data parsing here)
        std::string success = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n\r\n"
                              "<h1>POST request successful</h1>";
        send(client_socket, success.c_str(), success.length(), 0);
    }

    // Close the client connection
    close(client_socket);
}

void HttpServer::listenServer() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return; // Use return instead of exit for better error handling
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd); // Close the socket before returning
        return;
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd); // Close the socket before returning
        return;
    }

    std::cout << "Server started at port " << this->port << std::endl;

    while (true) {
        // Accept a new connection
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        if (client_socket < 0) {
            perror("accept failed");
            continue; // Continue accepting other connections on error
        }

        // Handle client in a new thread for concurrency
        std::thread t(&HttpServer::handle_client, this, client_socket);
        t.detach(); // Detach the thread to allow it to run independently
    }
}
