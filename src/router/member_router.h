#pragma once

#include "crow.h"
#include "../service/member_service.h"
#include "../middleware/access_log_middleware.h"
#include <string>

template<typename Middleware>
class MemberRouter {
private:
    crow::App<Middleware>& app;
    MemberService& memberService;

public:
    MemberRouter(crow::App<Middleware>& app, MemberService& service);
    
    // 멤버 관련 라우트들 설정
    void setupRoutes();
    
    // 개별 멤버 조회
    void getMember(const crow::request& req, crow::response& res, std::string id);
    
    // 모든 멤버 조회
    void getAllMembers(const crow::request& req, crow::response& res);
    
    // 멤버 생성
    void createMember(const crow::request& req, crow::response& res);
    
    // 멤버 업데이트
    void updateMember(const crow::request& req, crow::response& res, std::string id);
    
    // 멤버 삭제
    void deleteMember(const crow::request& req, crow::response& res, std::string id);
};
