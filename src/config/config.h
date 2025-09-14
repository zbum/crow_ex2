#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

struct DatabaseConfig {
    std::string host;
    int port;
    std::string username;
    std::string password;
    std::string database;
};

struct ServerConfig {
    std::string host;
    int port;
    int threads;
};

class Config {
private:
    DatabaseConfig dbConfig;
    ServerConfig serverConfig;
    
public:
    Config();
    Config(const std::string& configFile);
    
    // 설정 로드
    bool loadFromFile(const std::string& configFile);
    bool loadFromYaml(const YAML::Node& config);
    
    // Getters
    const DatabaseConfig& getDatabaseConfig() const { return dbConfig; }
    const ServerConfig& getServerConfig() const { return serverConfig; }
    
    // 기본값 설정
    void setDefaults();
    
    // 설정 검증
    bool validate() const;
};
