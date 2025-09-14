#include "member_service.h"

MemberService::MemberService(MySQLMemberRepository& repository) : memberRepository(repository) {
    // Repository는 생성자 매개변수로 전달받음
}

std::vector<crow::json::wvalue> MemberService::getAllMembers() {
    return memberRepository.getAllMembers();
}

crow::json::wvalue MemberService::getMemberById(const std::string& id) {
    if (!validateId(id)) {
        return crow::json::wvalue();
    }
    return memberRepository.getMemberById(id);
}

bool MemberService::memberExists(const std::string& id) {
    if (!validateId(id)) {
        return false;
    }
    return memberRepository.memberExists(id);
}

bool MemberService::addMember(const std::string& id, const crow::json::wvalue& member) {
    // 입력 검증
    if (!validateId(id) || !validateMember(member)) {
        return false;
    }
    
    // 중복 ID 체크
    if (memberRepository.memberExists(id)) {
        return false;
    }
    
    // 멤버 추가
    memberRepository.addMember(id, member);
    return true;
}

bool MemberService::updateMember(const std::string& id, const crow::json::wvalue& member) {
    // 입력 검증
    if (!validateId(id) || !validateMember(member)) {
        return false;
    }
    
    // 멤버 존재 체크
    if (!memberRepository.memberExists(id)) {
        return false;
    }
    
    // 멤버 업데이트
    memberRepository.updateMember(id, member);
    return true;
}

bool MemberService::deleteMember(const std::string& id) {
    // 입력 검증
    if (!validateId(id)) {
        return false;
    }
    
    // 멤버 존재 체크
    if (!memberRepository.memberExists(id)) {
        return false;
    }
    
    // 멤버 삭제
    memberRepository.deleteMember(id);
    return true;
}

bool MemberService::validateMember(const crow::json::wvalue& member) {
    // 필수 필드 존재 여부 검증
    if (member["name"].dump().empty() || member["gender"].dump().empty()) {
        return false;
    }
    
    // 이름 검증
    std::string name = member["name"].dump();
    if (name.length() <= 2) { // "name" 형태이므로 최소 3글자
        return false;
    }
    name = name.substr(1, name.length() - 2); // 따옴표 제거
    
    // 이름이 비어있거나 너무 긴 경우
    if (name.empty() || name.length() > 100) {
        return false;
    }
    
    // 이름에 허용되지 않는 문자 검증 (영문, 한글, 공백, 하이픈, 언더스코어만 허용)
    for (unsigned char c : name) {
        if (!std::isalnum(c) && c != ' ' && c != '-' && c != '_') {
            // 한글 범위 검증 (UTF-8 바이트 시퀀스 고려)
            bool isKorean = false;
            for (size_t i = 0; i < name.length(); i++) {
                unsigned char byte = static_cast<unsigned char>(name[i]);
                if (byte >= 0xEA && byte <= 0xED) { // 한글 UTF-8 범위
                    isKorean = true;
                    break;
                }
            }
            if (!isKorean) {
                return false;
            }
        }
    }
    
    // 성별 검증
    std::string gender = member["gender"].dump();
    if (gender.length() <= 2) { // "gender" 형태이므로 최소 3글자
        return false;
    }
    gender = gender.substr(1, gender.length() - 2); // 따옴표 제거
    
    // 성별이 허용된 값인지 검증
    if (gender != "male" && gender != "female") {
        return false;
    }
    
    return true;
}

bool MemberService::validateId(const std::string& id) {
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
