#include "config.h"
#include <iostream>
#include <fstream>

Config::Config() {
    setDefaults();
}

Config::Config(const std::string& configFile) {
    setDefaults();
    loadFromFile(configFile);
}

bool Config::loadFromFile(const std::string& configFile) {
    try {
        YAML::Node config = YAML::LoadFile(configFile);
        return loadFromYaml(config);
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading config file: " << e.what() << std::endl;
        return false;
    }
}

bool Config::loadFromYaml(const YAML::Node& config) {
    try {
        // Database 설정 로드
        if (config["database"]) {
            const auto& db = config["database"];
            if (db["host"]) dbConfig.host = db["host"].as<std::string>();
            if (db["port"]) dbConfig.port = db["port"].as<int>();
            if (db["username"]) dbConfig.username = db["username"].as<std::string>();
            if (db["password"]) dbConfig.password = db["password"].as<std::string>();
            if (db["database"]) dbConfig.database = db["database"].as<std::string>();
        }
        
        // Server 설정 로드
        if (config["server"]) {
            const auto& server = config["server"];
            if (server["host"]) serverConfig.host = server["host"].as<std::string>();
            if (server["port"]) serverConfig.port = server["port"].as<int>();
            if (server["threads"]) serverConfig.threads = server["threads"].as<int>();
        }
        
        return validate();
    } catch (const YAML::Exception& e) {
        std::cerr << "Error parsing YAML config: " << e.what() << std::endl;
        return false;
    }
}

void Config::setDefaults() {
    // Database 기본값
    dbConfig.host = "127.0.0.1";
    dbConfig.port = 3306;
    dbConfig.username = "root";
    dbConfig.password = "test";
    dbConfig.database = "crow_ex1";
    
    // Server 기본값
    serverConfig.host = "0.0.0.0";
    serverConfig.port = 8080;
    serverConfig.threads = 10;
}

bool Config::validate() const {
    // Database 설정 검증
    if (dbConfig.host.empty() || dbConfig.username.empty() || dbConfig.database.empty()) {
        std::cerr << "Invalid database configuration" << std::endl;
        return false;
    }
    
    if (dbConfig.port <= 0 || dbConfig.port > 65535) {
        std::cerr << "Invalid database port: " << dbConfig.port << std::endl;
        return false;
    }
    
    // Server 설정 검증
    if (serverConfig.port <= 0 || serverConfig.port > 65535) {
        std::cerr << "Invalid server port: " << serverConfig.port << std::endl;
        return false;
    }
    
    if (serverConfig.threads <= 0) {
        std::cerr << "Invalid thread count: " << serverConfig.threads << std::endl;
        return false;
    }
    
    return true;
}
