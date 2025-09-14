#define CROW_MAIN
#include "crow.h"
#include "src/middleware/access_log_middleware.h"
#include <iostream>

int main() {
    crow::App<AccessLogMiddleware> app;
    
    // 로그 레벨을 INFO로 설정
    app.loglevel(crow::LogLevel::Info);
    
    CROW_ROUTE(app, "/test")
    ([]() {
        return "Hello, World!";
    });
    
    CROW_ROUTE(app, "/delay")
    ([]() {
        // 100ms 지연
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return "Delayed response";
    });
    
    std::cout << "Starting server on port 8083..." << std::endl;
    std::cout << "Access logs will use Korean Standard Time (KST, UTC+9)" << std::endl;
    app.port(8083).multithreaded().run();
    
    return 0;
}
