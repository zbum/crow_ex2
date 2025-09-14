#pragma once

#include "crow.h"
#include "../service/product_service.h"
#include "../middleware/access_log_middleware.h"
#include <string>

template<typename Middleware>
class ProductRouter {
private:
    crow::App<Middleware>& app;
    ProductService& productService;

public:
    ProductRouter(crow::App<Middleware>& app, ProductService& service);
    
    // 제품 관련 라우트들 설정
    void setupRoutes();
    
    // 개별 제품 조회
    void getProduct(const crow::request& req, crow::response& res, std::string id);
    
    // 모든 제품 조회
    void getAllProducts(const crow::request& req, crow::response& res);
    
    // 제품 생성
    void createProduct(const crow::request& req, crow::response& res);
    
    // 제품 업데이트
    void updateProduct(const crow::request& req, crow::response& res, std::string id);
    
    // 제품 삭제
    void deleteProduct(const crow::request& req, crow::response& res, std::string id);
};
