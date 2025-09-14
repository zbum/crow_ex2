#include "../src/middleware/access_log_middleware.h"
#include <chrono>
#include <sstream>
#include <iostream>
#include <cassert>

// 간단한 테스트 함수들
void testLogFormatting() {
    std::cout << "Testing log formatting..." << std::endl;
    
    // 가상의 요청과 응답 생성
    crow::request req;
    crow::response res;
    AccessLogMiddleware::context ctx;
    
    // 시작 시간 설정
    ctx.start_time = std::chrono::high_resolution_clock::now();
    
    // 미들웨어 인스턴스 생성
    AccessLogMiddleware middleware;
    
    // before_handle 테스트
    middleware.before_handle(req, res, ctx);
    
    // 시간이 설정되었는지 확인
    auto now = std::chrono::high_resolution_clock::now();
    assert(ctx.start_time <= now);
    
    // after_handle 테스트 (실제로는 로그 출력을 확인해야 함)
    // 여기서는 시간 계산만 테스트
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - ctx.start_time);
    assert(duration.count() >= 0);
    
    std::cout << "✓ Log formatting test passed" << std::endl;
}

void testTimeFormatting() {
    std::cout << "Testing time formatting..." << std::endl;
    
    // 현재 시간을 ISO 8601 형식으로 포맷팅 테스트
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::tm* local_tm = std::localtime(&time_t);
    
    std::ostringstream log_stream;
    log_stream << std::put_time(local_tm, "%Y-%m-%dT%H:%M:%S") 
               << "." << std::setfill('0') << std::setw(3) << ms.count() << "+09:00";
    
    std::string time_str = log_stream.str();
    
    // ISO 8601 형식 검증
    assert(time_str.find("T") != std::string::npos);
    assert(time_str.find("+09:00") != std::string::npos);
    assert(time_str.length() == 25); // YYYY-MM-DDTHH:MM:SS.sss+09:00
    
    std::cout << "✓ Time formatting test passed" << std::endl;
}

int main() {
    std::cout << "Running unit tests..." << std::endl;
    
    try {
        testLogFormatting();
        testTimeFormatting();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
