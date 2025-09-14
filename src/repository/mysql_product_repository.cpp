#include "mysql_product_repository.h"

MySQLProductRepository::MySQLProductRepository(std::shared_ptr<MySQLConnectionPool> pool) : connectionPool(pool) {
}

std::vector<crow::json::wvalue> MySQLProductRepository::getAllProducts() {
    std::vector<crow::json::wvalue> products_list;
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), "SELECT id, name, price, category FROM products")) {
        std::cerr << "Error querying products: " << mysql_error(mysql.get()) << std::endl;
        return products_list;
    }
    
    MYSQL_RES* result = mysql_store_result(mysql.get());
    if (result == NULL) {
        std::cerr << "Error storing result: " << mysql_error(mysql.get()) << std::endl;
        return products_list;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        crow::json::wvalue product_obj;
        product_obj["id"] = std::string(row[0]);
        product_obj["name"] = std::string(row[1]);
        product_obj["price"] = std::stoi(row[2]);
        product_obj["category"] = std::string(row[3]);
        products_list.push_back(product_obj);
    }
    
    mysql_free_result(result);
    return products_list;
}

crow::json::wvalue MySQLProductRepository::getProductById(const std::string& id) {
    std::string query = "SELECT id, name, price, category FROM products WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error querying product: " << mysql_error(mysql.get()) << std::endl;
        return crow::json::wvalue();
    }
    
    MYSQL_RES* result = mysql_store_result(mysql.get());
    if (result == NULL) {
        std::cerr << "Error storing result: " << mysql_error(mysql.get()) << std::endl;
        return crow::json::wvalue();
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        crow::json::wvalue product_obj;
        product_obj["id"] = std::string(row[0]);
        product_obj["name"] = std::string(row[1]);
        product_obj["price"] = std::stoi(row[2]);
        product_obj["category"] = std::string(row[3]);
        mysql_free_result(result);
        return product_obj;
    }
    
    mysql_free_result(result);
    return crow::json::wvalue();
}

bool MySQLProductRepository::productExists(const std::string& id) {
    std::string query = "SELECT COUNT(*) FROM products WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error checking product existence: " << mysql_error(mysql.get()) << std::endl;
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(mysql.get());
    if (result == NULL) {
        std::cerr << "Error storing result: " << mysql_error(mysql.get()) << std::endl;
        return false;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = false;
    if (row != NULL) {
        exists = std::stoi(row[0]) > 0;
    }
    
    mysql_free_result(result);
    return exists;
}

void MySQLProductRepository::addProduct(const std::string& id, const crow::json::wvalue& product) {
    std::string name = std::string(product["name"].dump()).substr(1, std::string(product["name"].dump()).length() - 2);
    int price = std::stoi(std::string(product["price"].dump()));
    std::string category = std::string(product["category"].dump()).substr(1, std::string(product["category"].dump()).length() - 2);
    
    std::string query = "INSERT INTO products (id, name, price, category) VALUES ('" + id + "', '" + name + "', " + std::to_string(price) + ", '" + category + "')";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error adding product: " << mysql_error(mysql.get()) << std::endl;
    }
}

void MySQLProductRepository::updateProduct(const std::string& id, const crow::json::wvalue& product) {
    std::string name = std::string(product["name"].dump()).substr(1, std::string(product["name"].dump()).length() - 2);
    int price = std::stoi(std::string(product["price"].dump()));
    std::string category = std::string(product["category"].dump()).substr(1, std::string(product["category"].dump()).length() - 2);
    
    std::string query = "UPDATE products SET name = '" + name + "', price = " + std::to_string(price) + ", category = '" + category + "' WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error updating product: " << mysql_error(mysql.get()) << std::endl;
    }
}

void MySQLProductRepository::deleteProduct(const std::string& id) {
    std::string query = "DELETE FROM products WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error deleting product: " << mysql_error(mysql.get()) << std::endl;
    }
}