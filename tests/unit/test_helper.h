#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <functional>

class TestHelper {
private:
    std::vector<std::string> test_results;
    int passed_tests = 0;
    int total_tests = 0;
    
public:
    // 테스트 실행
    void runTest(const std::string& test_name, std::function<bool()> test_func) {
        total_tests++;
        std::cout << "Running test: " << test_name << " ... ";
        
        try {
            bool result = test_func();
            if (result) {
                std::cout << "PASS" << std::endl;
                passed_tests++;
                test_results.push_back(test_name + ": PASS");
            } else {
                std::cout << "FAIL" << std::endl;
                test_results.push_back(test_name + ": FAIL");
            }
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            test_results.push_back(test_name + ": ERROR - " + e.what());
        }
    }
    
    // 시간 측정 테스트
    void runTimedTest(const std::string& test_name, std::function<bool()> test_func, int max_ms = 1000) {
        total_tests++;
        std::cout << "Running timed test: " << test_name << " ... ";
        
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            bool result = test_func();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            if (result && duration.count() <= max_ms) {
                std::cout << "PASS (" << duration.count() << "ms)" << std::endl;
                passed_tests++;
                test_results.push_back(test_name + ": PASS (" + std::to_string(duration.count()) + "ms)");
            } else {
                std::cout << "FAIL (" << duration.count() << "ms)" << std::endl;
                test_results.push_back(test_name + ": FAIL (" + std::to_string(duration.count()) + "ms)");
            }
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            test_results.push_back(test_name + ": ERROR - " + e.what());
        }
    }
    
    // 결과 출력
    void printResults() {
        std::cout << "\n=== Test Results ===" << std::endl;
        for (const auto& result : test_results) {
            std::cout << result << std::endl;
        }
        std::cout << "\nTotal: " << passed_tests << "/" << total_tests << " tests passed" << std::endl;
    }
    
    // 성공 여부 반환
    bool allPassed() const {
        return passed_tests == total_tests;
    }
    
    // 통계 반환
    int getPassedCount() const { return passed_tests; }
    int getTotalCount() const { return total_tests; }
};
