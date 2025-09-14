#include "product_service.h"

ProductService::ProductService(MySQLProductRepository& repository) : productRepository(repository) {
    // Repository는 생성자 매개변수로 전달받음
}

std::vector<crow::json::wvalue> ProductService::getAllProducts() {
    return productRepository.getAllProducts();
}

crow::json::wvalue ProductService::getProductById(const std::string& id) {
    if (!validateId(id)) {
        return crow::json::wvalue();
    }
    return productRepository.getProductById(id);
}

bool ProductService::productExists(const std::string& id) {
    if (!validateId(id)) {
        return false;
    }
    return productRepository.productExists(id);
}

bool ProductService::addProduct(const std::string& id, const crow::json::wvalue& product) {
    // 입력 검증
    if (!validateId(id) || !validateProduct(product)) {
        return false;
    }
    
    // 중복 ID 체크
    if (productRepository.productExists(id)) {
        return false;
    }
    
    // 제품 추가
    productRepository.addProduct(id, product);
    return true;
}

bool ProductService::updateProduct(const std::string& id, const crow::json::wvalue& product) {
    // 입력 검증
    if (!validateId(id) || !validateProduct(product)) {
        return false;
    }
    
    // 제품 존재 체크
    if (!productRepository.productExists(id)) {
        return false;
    }
    
    // 제품 업데이트
    productRepository.updateProduct(id, product);
    return true;
}

bool ProductService::deleteProduct(const std::string& id) {
    // 입력 검증
    if (!validateId(id)) {
        return false;
    }
    
    // 제품 존재 체크
    if (!productRepository.productExists(id)) {
        return false;
    }
    
    // 제품 삭제
    productRepository.deleteProduct(id);
    return true;
}

bool ProductService::validateProduct(const crow::json::wvalue& product) {
    // 필수 필드 검증
    if (product["name"].dump().empty() || product["category"].dump().empty()) {
        return false;
    }
    
    // 이름 길이 검증
    std::string name = product["name"].dump();
    name = name.substr(1, name.length() - 2); // 따옴표 제거
    if (name.length() < 1 || name.length() > 100) {
        return false;
    }
    
    // 가격 검증
    std::string price_str = product["price"].dump();
    int price = std::stoi(price_str);
    if (price < 0 || price > 100000000) { // 0원 이상 1억원 이하
        return false;
    }
    
    // 카테고리 검증
    std::string category = product["category"].dump();
    category = category.substr(1, category.length() - 2); // 따옴표 제거
    if (category.length() < 1 || category.length() > 50) {
        return false;
    }
    
    return true;
}

bool ProductService::validateId(const std::string& id) {
    // ID 길이 검증
    if (id.empty() || id.length() > 50) {
        return false;
    }
    
    // ID 형식 검증 (영문자, 숫자, 하이픈, 언더스코어만 허용)
    for (char c : id) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}
