#ifndef RESPONSE_H
#define RESPONSE_H

class Response {

    private:
    int client_socket;
    public:
    
    Response(int socket);

    void send_response(const std::string& content, const std::string& content_type);
};

#endif // HTTP_SERVER_H
