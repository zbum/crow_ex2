-- 데이터베이스 생성
CREATE DATABASE IF NOT EXISTS crow_ex1;
USE crow_ex1;

-- 멤버 테이블 생성
CREATE TABLE IF NOT EXISTS members (
                                       id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    gender VARCHAR(20) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
    );

-- 제품 테이블 생성
CREATE TABLE IF NOT EXISTS products (
                                        id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price INT NOT NULL,
    category VARCHAR(50) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
    );

-- 멤버 데이터 삽입
INSERT INTO members (id, name, gender) VALUES
                                           ('1', '정지범', 'male'),
                                           ('2', '신진아', 'female');

-- 제품 데이터 삽입
INSERT INTO products (id, name, price, category) VALUES
                                                     ('p1', '노트북', 1500000, '전자제품'),
                                                     ('p2', '마우스', 50000, '전자제품'),
                                                     ('p3', '키보드', 100000, '전자제품');

-- 데이터 확인
SELECT 'Members:' as table_name;
SELECT * FROM members;
SELECT 'Products:' as table_name;
SELECT * FROM products;