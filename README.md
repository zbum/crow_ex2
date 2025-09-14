# Crow Framework Project

이 프로젝트는 [CrowCpp](https://github.com/CrowCpp/Crow) 웹 프레임워크를 사용한 C++ 웹 서버 애플리케이션입니다. MySQL 데이터베이스와 연동하여 회원 관리 및 상품 관리를 위한 RESTful API를 제공합니다.

## 기능

- **RESTful API 엔드포인트**: 회원 및 상품 관리 API
- **MySQL 데이터베이스 연동**: libmysql을 사용한 데이터베이스 연결
- **YAML 설정 파일**: 유연한 설정 관리
- **멀티스레드 지원**: 고성능 동시 처리
- **JSON 응답 지원**: 표준화된 API 응답 형식

## 프로젝트 구조

```
src/
├── main.cpp                    # 메인 애플리케이션
├── config/
│   ├── config.h               # 설정 클래스 헤더
│   └── config.cpp             # 설정 클래스 구현
├── router/
│   ├── member_router.h        # 회원 라우터 헤더
│   ├── member_router.cpp      # 회원 라우터 구현
│   ├── product_router.h       # 상품 라우터 헤더
│   └── product_router.cpp     # 상품 라우터 구현
├── service/
│   ├── member_service.h       # 회원 서비스 헤더
│   ├── member_service.cpp     # 회원 서비스 구현
│   ├── product_service.h      # 상품 서비스 헤더
│   └── product_service.cpp    # 상품 서비스 구현
└── repository/
    ├── mysql_member_repository.h    # 회원 리포지토리 헤더
    ├── mysql_member_repository.cpp  # 회원 리포지토리 구현
    ├── mysql_product_repository.h   # 상품 리포지토리 헤더
    ├── mysql_product_repository.cpp # 상품 리포지토리 구현
    └── mysql_connection_pool.h      # DB 연결 풀 헤더
```

## API 엔드포인트

### 회원 관리
- `GET /api/members` - 회원 목록 조회
- `GET /api/members/{id}` - 특정 회원 조회
- `POST /api/members` - 회원 등록
- `PUT /api/members/{id}` - 회원 정보 수정
- `DELETE /api/members/{id}` - 회원 삭제

### 상품 관리
- `GET /api/products` - 상품 목록 조회
- `GET /api/products/{id}` - 특정 상품 조회
- `POST /api/products` - 상품 등록
- `PUT /api/products/{id}` - 상품 정보 수정
- `DELETE /api/products/{id}` - 상품 삭제

## 빌드 및 실행

### 요구사항

- **CMake 3.15 이상**
- **C++17 지원 컴파일러** (AppleClang, GCC, MSVC)
- **vcpkg** (패키지 관리자)
- **MySQL 서버** (로컬 또는 원격)

### 의존성

이 프로젝트는 다음 라이브러리들을 사용합니다:
- **CrowCpp**: 웹 프레임워크
- **libmysql**: MySQL C API
- **yaml-cpp**: YAML 설정 파일 파싱
- **asio**: 비동기 I/O

### 환경 설정

1. **vcpkg 설치** (아직 설치하지 않은 경우):
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# 또는
.\bootstrap-vcpkg.bat  # Windows
```

2. **환경 변수 설정**:
```bash
export VCPKG_ROOT=/path/to/vcpkg
```

### 빌드 및 실행

#### 간단한 방법 (권장)
```bash
# 빌드 및 실행
make run

# 디버깅 모드 실행
make debug-lldb  # macOS
make debug       # Linux/Windows

# API 테스트
make test

# 정리
make clean
```

#### 수동 빌드
```bash
# CMake 설정
cmake --preset default

# 빌드
cmake --build --preset default

# 실행
cd build && ./bin/crow_ex2
```

### 설정 파일

프로젝트는 `config.yaml` 파일을 사용하여 설정을 관리합니다:

```yaml
server:
  host: "0.0.0.0"
  port: 8080
  threads: 10

database:
  host: "127.0.0.1"
  port: 3306
  name: "crow_ex1"
  user: "root"
  password: ""

logging:
  level: "info"
```

서버는 `http://localhost:8080`에서 실행됩니다.

## 테스트

### 자동 테스트 실행
```bash
# 모든 API 엔드포인트 테스트
make test
```

### 수동 API 테스트

#### 회원 관리 API
```bash
# 회원 목록 조회
curl http://localhost:8080/api/members

# 특정 회원 조회
curl http://localhost:8080/api/members/1

# 회원 등록
curl -X POST http://localhost:8080/api/members \
  -H "Content-Type: application/json" \
  -d '{"name": "홍길동", "email": "hong@example.com", "age": 30}'

# 회원 정보 수정
curl -X PUT http://localhost:8080/api/members/1 \
  -H "Content-Type: application/json" \
  -d '{"name": "홍길동", "email": "hong.updated@example.com", "age": 31}'

# 회원 삭제
curl -X DELETE http://localhost:8080/api/members/1
```

#### 상품 관리 API
```bash
# 상품 목록 조회
curl http://localhost:8080/api/products

# 특정 상품 조회
curl http://localhost:8080/api/products/1

# 상품 등록
curl -X POST http://localhost:8080/api/products \
  -H "Content-Type: application/json" \
  -d '{"name": "상품명", "price": 10000, "description": "상품 설명"}'

# 상품 정보 수정
curl -X PUT http://localhost:8080/api/products/1 \
  -H "Content-Type: application/json" \
  -d '{"name": "수정된 상품명", "price": 15000, "description": "수정된 상품 설명"}'

# 상품 삭제
curl -X DELETE http://localhost:8080/api/products/1
```

## 개발

### 디버깅
```bash
# LLDB 디버거로 실행 (macOS)
make debug-lldb

# GDB 디버거로 실행 (Linux/Windows)
make debug
```

### 코드 구조
- **Router**: HTTP 요청 라우팅 및 응답 처리
- **Service**: 비즈니스 로직 처리
- **Repository**: 데이터베이스 접근 계층
- **Config**: 설정 파일 관리

## 문제 해결

### 일반적인 문제들

1. **MySQL 연결 오류**: MySQL 서버가 실행 중인지 확인
2. **포트 충돌**: 8080 포트가 사용 중인 경우 `lsof -ti:8080 | xargs kill -9`
3. **vcpkg 오류**: `VCPKG_ROOT` 환경 변수가 올바르게 설정되었는지 확인

## 라이선스

BSD-3-Clause License
