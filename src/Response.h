#ifndef RESPONSE_H
#define RESPONSE_H

class Response {


    public:
    
    Response(int client_socket, const std::string& content, const std::string& content_type);
};

#endif // HTTP_SERVER_H
