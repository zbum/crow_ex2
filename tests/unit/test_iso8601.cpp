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
    
    std::cout << "Starting server on port 8082..." << std::endl;
    std::cout << "Access logs will use ISO 8601 format" << std::endl;
    app.port(8082).multithreaded().run();
    
    return 0;
}
