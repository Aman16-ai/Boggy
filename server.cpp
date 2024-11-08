#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>

#define PORT 8080

// Function to serve a client request
void handle_client(int client_socket);

// Main server loop
int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Server started at port " << PORT << std::endl;
    
    while (true) {
        // Accept a new connection
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        
        // Handle client in a new thread for concurrency
        std::thread t(handle_client, client_socket);
        t.detach();
    }
    
    return 0;
}

// Function to send HTTP responses
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

// Function to read and return file content
std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to handle incoming HTTP requests
void handle_client(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    
    std::string request(buffer);
    std::cout<<request<<std::endl;
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
        
        // Determine the content type
        std::string content_type = "text/html";
        if (file_path.find(".css") != std::string::npos) {
            content_type = "text/css";
        } else if (file_path.find(".js") != std::string::npos) {
            content_type = "application/javascript";
        }
        
        // Send the response
        if (!content.empty()) {
            send_response(client_socket, content, content_type);
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


// command : g++ -std=c++11 -pthread server.cpp -o server
// ./server
