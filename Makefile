# Crow 프로젝트 Makefile

.PHONY: all clean configure build run test

# 기본 타겟
all: configure build

# CMake 설정
configure:
	@echo "⚙️  CMake 설정..."
	cmake --preset default

# 빌드
build:
	@echo "🔨 빌드 실행..."
	cmake --build --preset default

# 전체 재빌드
rebuild: clean configure build

# 정리
clean:
	@echo "🧹 빌드 파일 정리..."
	rm -rf build

# 실행
run: build
	@echo "🚀 서버 실행..."
	cd build && ./bin/crow_ex2

# 백그라운드 실행
run-bg: build
	@echo "🚀 서버 백그라운드 실행..."
	cd build && ./bin/crow_ex2 &

# 디버깅 모드 빌드
debug-build:
	@echo "🐛 디버깅 모드 빌드..."
	rm -rf build
	mkdir -p build
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug
	cd build && make -j$(shell nproc 2>/dev/null || echo 4)

# 디버깅 모드 실행
debug: debug-build
	@echo "🐛 디버깅 모드 실행..."
	cd build && gdb ./bin/crow_ex2

# 디버깅 모드 실행 (lldb)
debug-lldb: debug-build
	@echo "🐛 디버깅 모드 실행 (lldb)..."
	cd build && lldb ./bin/crow_ex2

# 테스트
test: run-bg
	@echo "🧪 API 테스트..."
	sleep 2
	@echo "GET /"
	curl -s http://localhost:8080/
	@echo -e "\n\nGET /api/hello"
	curl -s http://localhost:8080/api/hello
	@echo -e "\n\nGET /api/users"
	curl -s http://localhost:8080/api/users
	@echo -e "\n\nPOST /api/echo"
	curl -s -X POST http://localhost:8080/api/echo \
		-H "Content-Type: application/json" \
		-d '{"message": "Hello World"}'
	@echo -e "\n\n테스트 완료!"

# 도움말
help:
	@echo "사용 가능한 명령어:"
	@echo "  make configure  - CMake 설정"
	@echo "  make build      - 빌드 실행"
	@echo "  make rebuild    - 전체 재빌드"
	@echo "  make run        - 서버 실행"
	@echo "  make run-bg     - 서버 백그라운드 실행"
	@echo "  make debug      - 디버깅 모드 실행 (gdb)"
	@echo "  make debug-lldb - 디버깅 모드 실행 (lldb)"
	@echo "  make test       - API 테스트"
	@echo "  make clean      - 빌드 파일 정리"
	@echo "  make help       - 도움말 표시"
