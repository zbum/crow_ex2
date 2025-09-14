#pragma once

#include "crow.h"
#include <chrono>
#include <iomanip>
#include <sstream>

struct AccessLogMiddleware
{
    struct context
    {
        std::chrono::high_resolution_clock::time_point start_time;
    };

    void before_handle(crow::request& /*req*/, crow::response& /*res*/, context& ctx)
    {
        // 요청 시작 시간 기록
        ctx.start_time = std::chrono::high_resolution_clock::now();
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        // 요청 종료 시간 기록
        auto end_time = std::chrono::high_resolution_clock::now();
        
        // 응답시간을 마이크로초 단위로 계산
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - ctx.start_time);
        
        // 현재 시간을 한국 시간(KST, UTC+9)으로 포맷팅 (ISO 8601 형식)
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::tm* local_tm = std::localtime(&time_t);
        
        // 로그 메시지 포맷팅 (ISO 8601 + Apache Common Log Format + 응답시간)
        std::ostringstream log_stream;
        log_stream << std::put_time(local_tm, "%Y-%m-%dT%H:%M:%S") 
                   << "." << std::setfill('0') << std::setw(3) << ms.count() << "+09:00"
                   << " " << req.remote_ip_address
                   << " \"" << static_cast<int>(req.method) << " " << req.url << " HTTP/1.1\""
                   << " " << res.code
                   << " " << (res.body.size() > 0 ? res.body.size() : 0)
                   << " " << duration.count() << "μs";
        
        // access log 출력
        CROW_LOG_INFO << "[ACCESS] " << log_stream.str();
    }
};