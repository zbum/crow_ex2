#pragma once

#include <mysql/mysql.h>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>
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
