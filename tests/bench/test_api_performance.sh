#!/bin/bash

# API 성능 테스트 스크립트
echo "=== HTTP API Performance Test ==="
echo "================================="

# 서버가 실행 중인지 확인
if ! curl -s http://localhost:8080/members > /dev/null; then
    echo "Error: Server is not running on localhost:8080"
    echo "Please start the server first with: make run"
    exit 1
fi

echo "Server is running. Starting performance tests..."
echo ""

# 1. 단일 요청 테스트
echo "1. Single Request Test:"
time curl -s http://localhost:8080/members > /dev/null
echo ""

# 2. 멀티플 요청 테스트
echo "2. Multiple Requests Test (10 requests):"
time for i in {1..10}; do
    curl -s http://localhost:8080/members > /dev/null
done
echo ""

# 3. 동시 요청 테스트
echo "3. Concurrent Requests Test (5 parallel requests):"
time {
    curl -s http://localhost:8080/members > /dev/null &
    curl -s http://localhost:8080/products > /dev/null &
    curl -s http://localhost:8080/members > /dev/null &
    curl -s http://localhost:8080/products > /dev/null &
    curl -s http://localhost:8080/members > /dev/null &
    wait
}
echo ""

# 4. POST 요청 테스트
echo "4. POST Request Test:"
time curl -s -X POST http://localhost:8080/members \
    -H "Content-Type: application/json" \
    -d '{"id":"perf_test","name":"Performance Test","email":"perf@test.com"}' > /dev/null
echo ""

# 5. GET by ID 테스트
echo "5. GET by ID Test:"
time curl -s http://localhost:8080/members/perf_test > /dev/null
echo ""

# 6. PUT 요청 테스트
echo "6. PUT Request Test:"
time curl -s -X PUT http://localhost:8080/members/perf_test \
    -H "Content-Type: application/json" \
    -d '{"id":"perf_test","name":"Updated Performance Test","email":"updated@test.com"}' > /dev/null
echo ""

# 7. DELETE 요청 테스트
echo "7. DELETE Request Test:"
time curl -s -X DELETE http://localhost:8080/members/perf_test > /dev/null
echo ""

echo "=== API Performance Test Completed ==="
