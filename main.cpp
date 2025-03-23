#include "src/HttpServer.h"
#include "src/Request.h"
#include "src/Response.h"
#include "src/Router.h"
#include <functional>
int main()
{

    HttpServer server(5000);
    Router router;
    server.setRouter(router);
    server.get("GET", "/about", [](Request &req, Response &res)
               { res.send_response("This is the about page", "text/html"); });

    server.get("GET", "/", [](Request &req, Response &res)
               { res.send_response("This is the home page", "text/html"); });

    server.get("GET", "/contact", [](Request &req, Response &res)
               { res.send_response("This is the contact Us page", "text/html"); });
    server.listenServer();
    return 0;
}