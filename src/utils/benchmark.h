#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

class Benchmark {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string test_name;
    
public:
    Benchmark(const std::string& name) : test_name(name) {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    ~Benchmark() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "[" << test_name << "] " << duration.count() << " microseconds" << std::endl;
    }
    
    // 중간 시간 측정
    void checkpoint(const std::string& checkpoint_name) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time);
        std::cout << "[" << test_name << " - " << checkpoint_name << "] " << duration.count() << " microseconds" << std::endl;
    }
};

// 매크로로 간편하게 사용
#define BENCHMARK(name) Benchmark _benchmark(name)
#define BENCHMARK_CHECKPOINT(name) _benchmark.checkpoint(name)
