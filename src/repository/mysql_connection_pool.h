#pragma once

#include <mysql/mysql.h>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <iostream>
#include "../config/config.h"

class MySQLConnectionPool {
private:
    std::queue<MYSQL*> available_connections;
    std::mutex pool_mutex;
    std::condition_variable pool_condition;
    DatabaseConfig dbConfig;
    size_t max_connections;
    size_t current_connections;

public:
    MySQLConnectionPool(const DatabaseConfig& config, size_t max_conn = 10) 
        : dbConfig(config), max_connections(max_conn), current_connections(0) {
    }

    // 데이터베이스 연결 테스트 및 초기화
    bool initialize() {
        std::cout << "Initializing database connection pool..." << std::endl;
        
        for (int attempt = 1; attempt <= dbConfig.max_retries; ++attempt) {
            std::cout << "Connection attempt " << attempt << "/" << dbConfig.max_retries << std::endl;
            
            MYSQL* test_conn = createConnection();
            if (test_conn) {
                std::cout << "Database connection successful!" << std::endl;
                mysql_close(test_conn);
                return true;
            }
            
            if (attempt < dbConfig.max_retries) {
                std::cout << "Connection failed. Retrying in " << dbConfig.retry_delay << " seconds..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(dbConfig.retry_delay));
            }
        }
        
        std::cerr << "Failed to connect to database after " << dbConfig.max_retries << " attempts" << std::endl;
        return false;
    }

    ~MySQLConnectionPool() {
        std::lock_guard<std::mutex> lock(pool_mutex);
        while (!available_connections.empty()) {
            MYSQL* conn = available_connections.front();
            available_connections.pop();
            mysql_close(conn);
        }
    }

    std::shared_ptr<MYSQL> getConnection() {
        std::unique_lock<std::mutex> lock(pool_mutex);
        
        // 사용 가능한 연결이 있으면 반환
        if (!available_connections.empty()) {
            MYSQL* conn = available_connections.front();
            available_connections.pop();
            return std::shared_ptr<MYSQL>(conn, [this](MYSQL* conn) {
                returnConnection(conn);
            });
        }
        
        // 새 연결 생성 가능하면 생성
        if (current_connections < max_connections) {
            MYSQL* conn = createConnection();
            if (conn) {
                current_connections++;
                return std::shared_ptr<MYSQL>(conn, [this](MYSQL* conn) {
                    returnConnection(conn);
                });
            }
        }
        
        // 연결이 없으면 대기
        pool_condition.wait(lock, [this] { return !available_connections.empty(); });
        
        MYSQL* conn = available_connections.front();
        available_connections.pop();
        return std::shared_ptr<MYSQL>(conn, [this](MYSQL* conn) {
            returnConnection(conn);
        });
    }

private:
    MYSQL* createConnection() {
        MYSQL* mysql = mysql_init(NULL);
        if (mysql == NULL) {
            std::cerr << "Error initializing MySQL" << std::endl;
            return nullptr;
        }
        
        // 연결 타임아웃 설정
        unsigned int timeout = dbConfig.connection_timeout;
        mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
        mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, &timeout);
        mysql_options(mysql, MYSQL_OPT_WRITE_TIMEOUT, &timeout);
        
        if (mysql_real_connect(mysql, dbConfig.host.c_str(), dbConfig.username.c_str(), 
                              dbConfig.password.c_str(), dbConfig.database.c_str(), 
                              dbConfig.port, NULL, 0) == NULL) {
            std::cerr << "Error connecting to MySQL: " << mysql_error(mysql) << std::endl;
            mysql_close(mysql);
            return nullptr;
        }
        
        return mysql;
    }

    void returnConnection(MYSQL* conn) {
        std::lock_guard<std::mutex> lock(pool_mutex);
        available_connections.push(conn);
        pool_condition.notify_one();
    }
};
