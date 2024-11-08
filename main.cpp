#include "src/HttpServer.h"

int main() {

    HttpServer server(5000);


    /*
    
        server.get("/about",std::function<void(Request,Response)> func)
    
    */
    server.listenServer();
    return 0;
}