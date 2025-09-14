#include "mysql_member_repository.h"

MySQLMemberRepository::MySQLMemberRepository(std::shared_ptr<MySQLConnectionPool> pool) : connectionPool(pool) {
}

std::vector<crow::json::wvalue> MySQLMemberRepository::getAllMembers() {
    std::vector<crow::json::wvalue> members_list;
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), "SELECT id, name, gender FROM members")) {
        std::cerr << "Error querying members: " << mysql_error(mysql.get()) << std::endl;
        return members_list;
    }
    
    MYSQL_RES* result = mysql_store_result(mysql.get());
    if (result == NULL) {
        std::cerr << "Error storing result: " << mysql_error(mysql.get()) << std::endl;
        return members_list;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        crow::json::wvalue member_obj;
        member_obj["id"] = std::string(row[0]);
        member_obj["name"] = std::string(row[1]);
        member_obj["gender"] = std::string(row[2]);
        members_list.push_back(member_obj);
    }
    
    mysql_free_result(result);
    return members_list;
}

crow::json::wvalue MySQLMemberRepository::getMemberById(const std::string& id) {
    std::string query = "SELECT id, name, gender FROM members WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error querying member: " << mysql_error(mysql.get()) << std::endl;
        return crow::json::wvalue();
    }
    
    MYSQL_RES* result = mysql_store_result(mysql.get());
    if (result == NULL) {
        std::cerr << "Error storing result: " << mysql_error(mysql.get()) << std::endl;
        return crow::json::wvalue();
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        crow::json::wvalue member_obj;
        member_obj["id"] = std::string(row[0]);
        member_obj["name"] = std::string(row[1]);
        member_obj["gender"] = std::string(row[2]);
        mysql_free_result(result);
        return member_obj;
    }
    
    mysql_free_result(result);
    return crow::json::wvalue();
}

bool MySQLMemberRepository::memberExists(const std::string& id) {
    std::string query = "SELECT COUNT(*) FROM members WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error checking member existence: " << mysql_error(mysql.get()) << std::endl;
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

void MySQLMemberRepository::addMember(const std::string& id, const crow::json::wvalue& member) {
    std::string name = std::string(member["name"].dump()).substr(1, std::string(member["name"].dump()).length() - 2);
    std::string gender = std::string(member["gender"].dump()).substr(1, std::string(member["gender"].dump()).length() - 2);
    
    std::string query = "INSERT INTO members (id, name, gender) VALUES ('" + id + "', '" + name + "', '" + gender + "')";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error adding member: " << mysql_error(mysql.get()) << std::endl;
    }
}

void MySQLMemberRepository::updateMember(const std::string& id, const crow::json::wvalue& member) {
    std::string name = std::string(member["name"].dump()).substr(1, std::string(member["name"].dump()).length() - 2);
    std::string gender = std::string(member["gender"].dump()).substr(1, std::string(member["gender"].dump()).length() - 2);
    
    std::string query = "UPDATE members SET name = '" + name + "', gender = '" + gender + "' WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error updating member: " << mysql_error(mysql.get()) << std::endl;
    }
}

void MySQLMemberRepository::deleteMember(const std::string& id) {
    std::string query = "DELETE FROM members WHERE id = '" + id + "'";
    auto mysql = connectionPool->getConnection();
    
    if (mysql_query(mysql.get(), query.c_str())) {
        std::cerr << "Error deleting member: " << mysql_error(mysql.get()) << std::endl;
    }
}