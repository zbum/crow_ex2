#pragma once

#include "crow.h"
#include "../repository/mysql_product_repository.h"
#include <string>
#include <vector>

class ProductService {
private:
    MySQLProductRepository& productRepository;

public:
    ProductService(MySQLProductRepository& repository);
    
    // 모든 제품 조회
    std::vector<crow::json::wvalue> getAllProducts();
    
    // ID로 제품 조회
    crow::json::wvalue getProductById(const std::string& id);
    
    // 제품 존재 여부 확인
    bool productExists(const std::string& id);
    
    // 제품 추가
    bool addProduct(const std::string& id, const crow::json::wvalue& product);
    
    // 제품 업데이트
    bool updateProduct(const std::string& id, const crow::json::wvalue& product);
    
    // 제품 삭제
    bool deleteProduct(const std::string& id);

private:
    // 제품 데이터 검증
    bool validateProduct(const crow::json::wvalue& product);
    
    // ID 검증
    bool validateId(const std::string& id);
};
