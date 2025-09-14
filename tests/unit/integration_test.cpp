#include <iostream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <string>
#include <sstream>

// cURL 콜백 함수
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    }
    catch(std::bad_alloc &e) {
        return 0;
    }
}

class IntegrationTest {
private:
    std::string server_url;
    CURL* curl;
    
public:
    IntegrationTest(const std::string& url) : server_url(url) {
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize cURL");
        }
    }
    
    ~IntegrationTest() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
    
    // HTTP GET 요청 테스트
    bool testGet(const std::string& endpoint, int expected_status = 200) {
        std::string response;
        std::string url = server_url + endpoint;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        
        CURLcode res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
        
        long http_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        
        std::cout << "GET " << endpoint << " -> Status: " << http_code << std::endl;
        return http_code == expected_status;
    }
    
    // HTTP POST 요청 테스트
    bool testPost(const std::string& endpoint, const std::string& data, int expected_status = 200) {
        std::string response;
        std::string url = server_url + endpoint;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        
        // JSON 헤더 설정
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        CURLcode res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            curl_slist_free_all(headers);
            return false;
        }
        
        long http_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        
        std::cout << "POST " << endpoint << " -> Status: " << http_code << std::endl;
        
        curl_slist_free_all(headers);
        return http_code == expected_status;
    }
    
    // 응답 시간 측정 테스트
    bool testResponseTime(const std::string& endpoint, int max_ms = 1000) {
        auto start = std::chrono::high_resolution_clock::now();
        
        bool success = testGet(endpoint);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Response time: " << duration.count() << "ms" << std::endl;
        
        return success && duration.count() <= max_ms;
    }
};

int main() {
    std::cout << "=== Integration Test for Crow Server ===" << std::endl;
    
    // 서버가 실행 중인지 확인
    std::cout << "Testing server at http://localhost:3000" << std::endl;
    
    IntegrationTest test("http://localhost:3000");
    
    // 테스트 케이스들
    std::cout << "\n1. Testing GET /members" << std::endl;
    bool test1 = test.testGet("/members");
    
    std::cout << "\n2. Testing GET /products" << std::endl;
    bool test2 = test.testGet("/products");
    
    std::cout << "\n3. Testing POST /members" << std::endl;
    std::string member_data = R"({"id":"test123","name":"테스트사용자","gender":"male"})";
    bool test3 = test.testPost("/members", member_data, 201);
    
    std::cout << "\n4. Testing response time" << std::endl;
    bool test4 = test.testResponseTime("/members", 500);
    
    // 결과 요약
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "GET /members: " << (test1 ? "PASS" : "FAIL") << std::endl;
    std::cout << "GET /products: " << (test2 ? "PASS" : "FAIL") << std::endl;
    std::cout << "POST /members: " << (test3 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Response time: " << (test4 ? "PASS" : "FAIL") << std::endl;
    
    int passed = (test1 ? 1 : 0) + (test2 ? 1 : 0) + (test3 ? 1 : 0) + (test4 ? 1 : 0);
    std::cout << "\nTotal: " << passed << "/4 tests passed" << std::endl;
    
    return (passed == 4) ? 0 : 1;
}
