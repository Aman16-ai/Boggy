# C++ Web Development Framework

**Fast, unopinionated, minimalist C++ Web development framework.**

## Objective
The goal of this framework is to fill the gap of a **minimalist C++ web framework** for **rapid API and web development** while ensuring **high performance**.

## Features
- Lightweight and minimalistic
- High-performance HTTP server
- Easy-to-use routing system
- Designed for rapid API and web development

## Example Usage
```cpp
#include "src/HttpServer.h"
#include "src/Request.h"
#include "src/Response.h"
#include "src/Router.h"
#include <functional>

int main() {
    HttpServer server(5000);
    Router router;
    server.setRouter(router);


    server.get("/", [](Request &req, Response &res) {
        res.send_response("This is the home page", "text/html");
    });

    server.get("/about", [](Request &req, Response &res) {
        res.send_response("This is the about page", "text/html");
    });

    server.get("/contact", [](Request &req, Response &res) {
        res.send_response("This is the contact Us page", "text/html");
    });

    server.listenServer();
    return 0;
}
```
## Status
🚧 **Under Development**  
Currently, this framework works on Linux.

