# Crow Framework Project

이 프로젝트는 [CrowCpp](https://github.com/CrowCpp/Crow) 웹 프레임워크를 사용한 C++ 웹 서버 예제입니다.

## 기능

- RESTful API 엔드포인트
- JSON 응답 지원
- CORS 미들웨어
- 멀티스레드 지원

## API 엔드포인트

- `GET /` - 기본 인사 메시지
- `GET /api/hello` - JSON 형태의 인사 메시지
- `POST /api/echo` - 요청 데이터를 그대로 반환
- `GET /api/users` - 사용자 목록 반환

## 빌드 및 실행

### 요구사항

- CMake 3.20 이상
- C++17 지원 컴파일러
- vcpkg (패키지 관리자)

### 빌드

```bash
# vcpkg를 사용하여 의존성 설치
vcpkg install crow

# CMake로 빌드
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
make
```

### 실행

```bash
./bin/crow_ex2
```

서버는 `http://localhost:8080`에서 실행됩니다.

## 테스트

### curl을 사용한 테스트

```bash
# 기본 엔드포인트
curl http://localhost:8080/

# API 엔드포인트
curl http://localhost:8080/api/hello

# 사용자 목록
curl http://localhost:8080/api/users

# Echo 테스트
curl -X POST http://localhost:8080/api/echo \
  -H "Content-Type: application/json" \
  -d '{"message": "Hello World"}'
```

## 라이선스

BSD-3-Clause License
