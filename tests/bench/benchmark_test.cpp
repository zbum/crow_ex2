#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include "../../src/utils/benchmark.h"
#include "../../src/config/config.h"
#include "../../src/repository/mysql_member_repository.h"
#include "../../src/repository/mysql_product_repository.h"
#include "../../src/service/member_service.h"
#include "../../src/service/product_service.h"

// 간단한 성능 테스트 함수들
void testDatabaseConnection() {
    BENCHMARK("Database Connection Test");
    
    Config config;
    config.setDefaults();
    
    BENCHMARK_CHECKPOINT("Config loaded");
    
    MySQLMemberRepository memberRepo(config.getDatabaseConfig());
    BENCHMARK_CHECKPOINT("Member repository created");
    
    MySQLProductRepository productRepo(config.getDatabaseConfig());
    BENCHMARK_CHECKPOINT("Product repository created");
}

void testServiceOperations() {
    BENCHMARK("Service Operations Test");
    
    Config config;
    config.setDefaults();
    
    MySQLMemberRepository memberRepo(config.getDatabaseConfig());
    MySQLProductRepository productRepo(config.getDatabaseConfig());
    
    MemberService memberService(memberRepo);
    ProductService productService(productRepo);
    
    BENCHMARK_CHECKPOINT("Services created");
    
    // 멤버 조회 테스트
    try {
        auto members = memberService.getAllMembers();
        BENCHMARK_CHECKPOINT("Get all members");
        std::cout << "Found " << members.size() << " members" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Member service error: " << e.what() << std::endl;
    }
    
    // 제품 조회 테스트
    try {
        auto products = productService.getAllProducts();
        BENCHMARK_CHECKPOINT("Get all products");
        std::cout << "Found " << products.size() << " products" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Product service error: " << e.what() << std::endl;
    }
}

void testConcurrentOperations() {
    BENCHMARK("Concurrent Operations Test");
    
    const int num_threads = 4;
    const int operations_per_thread = 10;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i]() {
            // 각 스레드마다 독립적인 연결 생성
            Config config;
            config.setDefaults();
            
            MySQLMemberRepository memberRepo(config.getDatabaseConfig());
            MemberService memberService(memberRepo);
            
            for (int j = 0; j < operations_per_thread; ++j) {
                try {
                    auto members = memberService.getAllMembers();
                    if (j == 0) {
                        std::cout << "Thread " << i << " found " << members.size() << " members" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Thread " << i << " error: " << e.what() << std::endl;
                }
            }
        });
    }
    
    BENCHMARK_CHECKPOINT("Threads started");
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    BENCHMARK_CHECKPOINT("All threads completed");
}

void testMemoryUsage() {
    BENCHMARK("Memory Usage Test");
    
    std::vector<std::string> large_data;
    const int data_size = 10000;
    
    for (int i = 0; i < data_size; ++i) {
        large_data.push_back("Test data string number " + std::to_string(i));
    }
    
    BENCHMARK_CHECKPOINT("Large data created");
    
    // 데이터 처리 시뮬레이션
    int total_length = 0;
    for (const auto& str : large_data) {
        total_length += str.length();
    }
    
    BENCHMARK_CHECKPOINT("Data processed");
    std::cout << "Total string length: " << total_length << std::endl;
}

void testSimpleOperations() {
    BENCHMARK("Simple Operations Test");
    
    // 간단한 연산 테스트
    std::vector<int> numbers;
    for (int i = 0; i < 100000; ++i) {
        numbers.push_back(i);
    }
    
    BENCHMARK_CHECKPOINT("Numbers created");
    
    // 정렬 테스트
    std::sort(numbers.begin(), numbers.end());
    BENCHMARK_CHECKPOINT("Numbers sorted");
    
    // 검색 테스트
    auto it = std::find(numbers.begin(), numbers.end(), 50000);
    BENCHMARK_CHECKPOINT("Number found");
    
    std::cout << "Found number: " << (it != numbers.end() ? "Yes" : "No") << std::endl;
}

int main() {
    std::cout << "=== C++ Performance Benchmark Test ===" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // 1. 데이터베이스 연결 테스트
    std::cout << "\n1. Database Connection Test:" << std::endl;
    testDatabaseConnection();
    
    // 2. 서비스 작업 테스트
    std::cout << "\n2. Service Operations Test:" << std::endl;
    testServiceOperations();
    
    // 3. 동시성 테스트
    std::cout << "\n3. Concurrent Operations Test:" << std::endl;
    testConcurrentOperations();
    
    // 4. 메모리 사용량 테스트
    std::cout << "\n4. Memory Usage Test:" << std::endl;
    testMemoryUsage();
    
    // 5. 간단한 연산 테스트
    std::cout << "\n5. Simple Operations Test:" << std::endl;
    testSimpleOperations();
    
    std::cout << "\n=== Benchmark Test Completed ===" << std::endl;
    return 0;
}
