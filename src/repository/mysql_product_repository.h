#pragma once

#include "crow.h"
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <memory>
#include "../config/config.h"
#include "mysql_connection_pool.h"

class MySQLProductRepository {
private:
    std::shared_ptr<MySQLConnectionPool> connectionPool;

public:
    MySQLProductRepository(std::shared_ptr<MySQLConnectionPool> pool);
    ~MySQLProductRepository() = default;
    
    // 모든 제품 조회
    std::vector<crow::json::wvalue> getAllProducts();
    
    // ID로 제품 조회
    crow::json::wvalue getProductById(const std::string& id);
    
    // 제품 존재 여부 확인
    bool productExists(const std::string& id);
    
    // 제품 추가
    void addProduct(const std::string& id, const crow::json::wvalue& product);
    
    // 제품 업데이트
    void updateProduct(const std::string& id, const crow::json::wvalue& product);
    
    // 제품 삭제
    void deleteProduct(const std::string& id);
};
