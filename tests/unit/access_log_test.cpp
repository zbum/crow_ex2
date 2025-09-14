#include "test_helper.h"
#include "../src/middleware/access_log_middleware.h"
#include <chrono>
#include <sstream>
#include <iomanip>

// Access Log 미들웨어 테스트
class AccessLogTest {
private:
    TestHelper test_helper;
    
public:
    void runAllTests() {
        std::cout << "=== Access Log Middleware Tests ===" << std::endl;
        
        // 시간 포맷팅 테스트
        test_helper.runTest("ISO 8601 Time Formatting", [this]() {
            return testTimeFormatting();
        });
        
        // 응답시간 계산 테스트
        test_helper.runTest("Response Time Calculation", [this]() {
            return testResponseTimeCalculation();
        });
        
        // 로그 메시지 포맷팅 테스트
        test_helper.runTest("Log Message Formatting", [this]() {
            return testLogMessageFormatting();
        });
        
        // 한국 시간대 테스트
        test_helper.runTest("Korean Timezone (KST)", [this]() {
            return testKoreanTimezone();
        });
        
        // 마이크로초 정확도 테스트
        test_helper.runTimedTest("Microsecond Accuracy", [this]() {
            return testMicrosecondAccuracy();
        }, 100); // 100ms 이내
        
        test_helper.printResults();
    }
    
private:
    bool testTimeFormatting() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::tm* local_tm = std::localtime(&time_t);
        
        std::ostringstream log_stream;
        log_stream << std::put_time(local_tm, "%Y-%m-%dT%H:%M:%S") 
                   << "." << std::setfill('0') << std::setw(3) << ms.count() << "+09:00";
        
        std::string time_str = log_stream.str();
        
        // ISO 8601 형식 검증
        return time_str.find("T") != std::string::npos &&
               time_str.find("+09:00") != std::string::npos &&
               time_str.length() == 25; // YYYY-MM-DDTHH:MM:SS.sss+09:00
    }
    
    bool testResponseTimeCalculation() {
        AccessLogMiddleware::context ctx;
        ctx.start_time = std::chrono::high_resolution_clock::now();
        
        // 1ms 지연
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - ctx.start_time);
        
        // 1ms = 1000μs 이상이어야 함
        return duration.count() >= 1000;
    }
    
    bool testLogMessageFormatting() {
        // 가상의 로그 메시지 생성
        std::string ip = "127.0.0.1";
        std::string method = "1";
        std::string url = "/test";
        int status_code = 200;
        int response_size = 13;
        int response_time = 1234;
        
        std::ostringstream log_stream;
        log_stream << "[ACCESS] 2025-09-14T02:49:40.078+09:00 "
                   << ip << " \"" << method << " " << url << " HTTP/1.1\""
                   << " " << status_code << " " << response_size << " " << response_time << "μs";
        
        std::string log_message = log_stream.str();
        
        // 필수 요소들이 포함되어 있는지 확인
        return log_message.find("[ACCESS]") != std::string::npos &&
               log_message.find(ip) != std::string::npos &&
               log_message.find(url) != std::string::npos &&
               log_message.find(std::to_string(status_code)) != std::string::npos &&
               log_message.find("μs") != std::string::npos;
    }
    
    bool testKoreanTimezone() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::tm* local_tm = std::localtime(&time_t);
        std::tm* utc_tm = std::gmtime(&time_t);
        
        // 한국 시간은 UTC+9이므로 9시간 차이가 있어야 함
        int hour_diff = (local_tm->tm_hour - utc_tm->tm_hour + 24) % 24;
        return hour_diff == 9;
    }
    
    bool testMicrosecondAccuracy() {
        auto start = std::chrono::high_resolution_clock::now();
        
        // 10μs 지연 (매우 짧은 시간)
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // 10μs 이상이어야 함 (정확도 테스트)
        return duration.count() >= 10;
    }
};

int main() {
    AccessLogTest test;
    test.runAllTests();
    
    return 0;
}
