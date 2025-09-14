#define CROW_MAIN
#include "crow.h"
#include "router/member_router.h"
#include "router/product_router.h"
#include "service/member_service.h"
#include "service/product_service.h"
#include "repository/mysql_member_repository.h"
#include "repository/mysql_product_repository.h"
#include "repository/mysql_connection_pool.h"
#include "config/config.h"
#include "middleware/access_log_middleware.h"
#include <iostream>
#include <memory>

int main(const int argc, char* argv[]) {
    // 설정 파일 경로 (기본값: config.yaml)
    std::string configFile = "config.yaml";
    if (argc > 1) {
        configFile = argv[1];
    }
    
    // 설정 로드
    Config config;
    if (!config.loadFromFile(configFile)) {
        std::cerr << "Failed to load config file: " << configFile << std::endl;
        std::cerr << "Using default configuration..." << std::endl;
        config.setDefaults();
    }
    
    std::cout << "Configuration loaded from: " << configFile << std::endl;
    std::cout << "Database: " << config.getDatabaseConfig().host << ":" 
              << config.getDatabaseConfig().port << "/" << config.getDatabaseConfig().database << std::endl;
    std::cout << "Server: " << config.getServerConfig().host << ":" 
              << config.getServerConfig().port << " (threads: " << config.getServerConfig().threads << ")" << std::endl;
    
    // MySQL 연결 풀 생성 및 초기화
    auto connectionPool = std::make_shared<MySQLConnectionPool>(config.getDatabaseConfig(), 10);
    
    // 데이터베이스 연결 확인
    if (!connectionPool->initialize()) {
        std::cerr << "Failed to initialize database connection. Exiting..." << std::endl;
        return 1;
    }
    
    std::cout << "Database connection pool initialized successfully!" << std::endl;
    
    crow::App<AccessLogMiddleware> app;
    
    // Repository 인스턴스 생성 (연결 풀 전달)
    MySQLMemberRepository memberRepository(connectionPool);
    MySQLProductRepository productRepository(connectionPool);
    
    // Service 인스턴스 생성 (Repository 참조 전달)
    MemberService memberService(memberRepository);
    ProductService productService(productRepository);
    
    // 각 도메인별 라우터 생성 및 라우트 설정 (Service 참조 전달)
    MemberRouter<AccessLogMiddleware> memberRouter(app, memberService);
    memberRouter.setupRoutes();
    
    ProductRouter<AccessLogMiddleware> productRouter(app, productService);
    productRouter.setupRoutes();
    
    // 서버 시작 (설정된 포트와 스레드 수 사용)
    app.port(config.getServerConfig().port)
       .multithreaded()
       .run();
    return 0;
}
