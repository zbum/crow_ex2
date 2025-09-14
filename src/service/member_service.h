#pragma once

#include "crow.h"
#include "../repository/mysql_member_repository.h"
#include <string>
#include <vector>

class MemberService {
private:
    MySQLMemberRepository& memberRepository;

public:
    MemberService(MySQLMemberRepository& repository);
    
    // 모든 멤버 조회
    std::vector<crow::json::wvalue> getAllMembers();
    
    // ID로 멤버 조회
    crow::json::wvalue getMemberById(const std::string& id);
    
    // 멤버 존재 여부 확인
    bool memberExists(const std::string& id);
    
    // 멤버 추가
    bool addMember(const std::string& id, const crow::json::wvalue& member);
    
    // 멤버 업데이트
    bool updateMember(const std::string& id, const crow::json::wvalue& member);
    
    // 멤버 삭제
    bool deleteMember(const std::string& id);

private:
    // 멤버 데이터 검증
    bool validateMember(const crow::json::wvalue& member);
    
    // ID 검증
    bool validateId(const std::string& id);
};
