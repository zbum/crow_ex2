#include "member_router.h"

template<typename Middleware>
MemberRouter<Middleware>::MemberRouter(crow::App<Middleware>& app, MemberService& service) : app(app), memberService(service) {
    // Service는 생성자 매개변수로 전달받음
}

template<typename Middleware>
void MemberRouter<Middleware>::setupRoutes() {
    // 모든 멤버 조회 라우트 (GET)
    CROW_ROUTE(app, "/members")
    .methods("GET"_method)
    ([this](const crow::request& req, crow::response& res){
        getAllMembers(req, res);
    });

    // 멤버 생성 라우트 (POST)
    CROW_ROUTE(app, "/members")
    .methods("POST"_method)
    ([this](const crow::request& req, crow::response& res){
        createMember(req, res);
    });

    // 개별 멤버 조회 라우트 (GET)
    CROW_ROUTE(app, "/members/<string>")
    .methods("GET"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        getMember(req, res, id);
    });

    // 멤버 업데이트 라우트 (PUT)
    CROW_ROUTE(app, "/members/<string>")
    .methods("PUT"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        updateMember(req, res, id);
    });

    // 멤버 삭제 라우트 (DELETE)
    CROW_ROUTE(app, "/members/<string>")
    .methods("DELETE"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        deleteMember(req, res, id);
    });
}

template<typename Middleware>
void MemberRouter<Middleware>::getMember(const crow::request& /*req*/, crow::response& res, std::string id) {
    if (memberService.memberExists(id)) {
        auto member = memberService.getMemberById(id);
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(member.dump());
    } else {
        res.code = 404;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Member not found"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void MemberRouter<Middleware>::getAllMembers(const crow::request& /*req*/, crow::response& res) {
    // Service에서 모든 멤버 조회
    auto members_list = memberService.getAllMembers();
    
    // vector를 wvalue로 변환 (복사 대신 이동 시맨틱 사용)
    crow::json::wvalue members_array;
    members_array = std::move(members_list);
    
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write(members_array.dump());
    res.end();
}

template<typename Middleware>
void MemberRouter<Middleware>::createMember(const crow::request& req, crow::response& res) {
    try {
        // Content-Type 검증
        if (req.get_header_value("Content-Type") != "application/json") {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Content-Type must be application/json"}
            }).dump());
            res.end();
            return;
        }

        // JSON 파싱
        auto json_data = crow::json::load(req.body);
        if (json_data.error()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Invalid JSON format"}
            }).dump());
            res.end();
            return;
        }

        // 필수 필드 존재 여부 검증
        if (!json_data.has("id") || !json_data.has("name") || !json_data.has("gender")) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Missing required fields: id, name, gender"}
            }).dump());
            res.end();
            return;
        }

        // ID 추출 및 검증
        std::string id = json_data["id"].s();
        if (id.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "ID cannot be empty"}
            }).dump());
            res.end();
            return;
        }

        // 이름 추출 및 검증
        std::string name = json_data["name"].s();
        if (name.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Name cannot be empty"}
            }).dump());
            res.end();
            return;
        }

        // 성별 추출 및 검증
        std::string gender = json_data["gender"].s();
        if (gender.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Gender cannot be empty"}
            }).dump());
            res.end();
            return;
        }

        // 멤버 데이터 생성
        crow::json::wvalue member_data;
        member_data["name"] = name;
        member_data["gender"] = gender;

        // Service를 통한 멤버 생성
        if (memberService.addMember(id, member_data)) {
            res.code = 201;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"message", "Member created successfully"},
                {"id", id}
            }).dump());
        } else {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Member already exists or invalid data format"}
            }).dump());
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Internal server error"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void MemberRouter<Middleware>::updateMember(const crow::request& req, crow::response& res, std::string id) {
    try {
        // Content-Type 검증
        if (req.get_header_value("Content-Type") != "application/json") {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Content-Type must be application/json"}
            }).dump());
            res.end();
            return;
        }

        // JSON 파싱
        auto json_data = crow::json::load(req.body);
        if (json_data.error()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Invalid JSON format"}
            }).dump());
            res.end();
            return;
        }

        // 필수 필드 존재 여부 검증
        if (!json_data.has("name") || !json_data.has("gender")) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Missing required fields: name, gender"}
            }).dump());
            res.end();
            return;
        }

        // 이름 추출 및 검증
        std::string name = json_data["name"].s();
        if (name.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Name cannot be empty"}
            }).dump());
            res.end();
            return;
        }

        // 성별 추출 및 검증
        std::string gender = json_data["gender"].s();
        if (gender.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Gender cannot be empty"}
            }).dump());
            res.end();
            return;
        }

        // 멤버 데이터 생성
        crow::json::wvalue member_data;
        member_data["name"] = name;
        member_data["gender"] = gender;

        // Service를 통한 멤버 업데이트
        if (memberService.updateMember(id, member_data)) {
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"message", "Member updated successfully"},
                {"id", id}
            }).dump());
        } else {
            res.code = 404;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Member not found or invalid data format"}
            }).dump());
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Internal server error"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void MemberRouter<Middleware>::deleteMember(const crow::request& /*req*/, crow::response& res, std::string id) {
    // Service를 통한 멤버 삭제
    if (memberService.deleteMember(id)) {
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"message", "Member deleted successfully"},
            {"id", id}
        }).dump());
    } else {
        res.code = 404;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Member not found"}
        }).dump());
    }
    res.end();
}

// 명시적 인스턴스 선언
template class MemberRouter<struct AccessLogMiddleware>;
