#include "product_router.h"

template<typename Middleware>
ProductRouter<Middleware>::ProductRouter(crow::App<Middleware>& app, ProductService& service) : app(app), productService(service) {
    // Service는 생성자 매개변수로 전달받음
}

template<typename Middleware>
void ProductRouter<Middleware>::setupRoutes() {
    // 모든 제품 조회 라우트 (GET)
    CROW_ROUTE(app, "/products")
    .methods("GET"_method)
    ([this](const crow::request& req, crow::response& res){
        getAllProducts(req, res);
    });

    // 제품 생성 라우트 (POST)
    CROW_ROUTE(app, "/products")
    .methods("POST"_method)
    ([this](const crow::request& req, crow::response& res){
        createProduct(req, res);
    });

    // 개별 제품 조회 라우트 (GET)
    CROW_ROUTE(app, "/products/<string>")
    .methods("GET"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        getProduct(req, res, id);
    });

    // 제품 업데이트 라우트 (PUT)
    CROW_ROUTE(app, "/products/<string>")
    .methods("PUT"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        updateProduct(req, res, id);
    });

    // 제품 삭제 라우트 (DELETE)
    CROW_ROUTE(app, "/products/<string>")
    .methods("DELETE"_method)
    ([this](const crow::request& req, crow::response& res, std::string id){
        deleteProduct(req, res, id);
    });
}

template<typename Middleware>
void ProductRouter<Middleware>::getProduct(const crow::request& /*req*/, crow::response& res, std::string id) {
    if (productService.productExists(id)) {
        auto product = productService.getProductById(id);
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(product.dump());
    } else {
        res.code = 404;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Product not found"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void ProductRouter<Middleware>::getAllProducts(const crow::request& /*req*/, crow::response& res) {
    // Service에서 모든 제품 조회
    auto products_list = productService.getAllProducts();
    
    // vector를 wvalue로 변환
    crow::json::wvalue products_array(std::move(products_list));
    
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write(products_array.dump());
    res.end();
}

template<typename Middleware>
void ProductRouter<Middleware>::createProduct(const crow::request& req, crow::response& res) {
    try {
        // JSON 파싱
        auto json_data = crow::json::load(req.body);
        if (json_data.error()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Invalid JSON format"}
            }).dump());
            res.end();
            return;
        }

        // ID 추출
        std::string id = json_data["id"].s();
        if (id.empty()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "ID is required"}
            }).dump());
            res.end();
            return;
        }

        // 제품 데이터 생성
        crow::json::wvalue product_data;
        product_data["name"] = json_data["name"].s();
        product_data["price"] = json_data["price"].i();
        product_data["category"] = json_data["category"].s();

        // Service를 통한 제품 생성
        if (productService.addProduct(id, product_data)) {
            res.code = 201;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"message", "Product created successfully"},
                {"id", id}
            }).dump());
        } else {
            res.code = 409;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Product already exists or invalid data"}
            }).dump());
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Internal server error"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void ProductRouter<Middleware>::updateProduct(const crow::request& req, crow::response& res, std::string id) {
    try {
        // JSON 파싱
        auto json_data = crow::json::load(req.body);
        if (json_data.error()) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Invalid JSON format"}
            }).dump());
            res.end();
            return;
        }

        // 제품 데이터 생성
        crow::json::wvalue product_data;
        product_data["name"] = json_data["name"].s();
        product_data["price"] = json_data["price"].i();
        product_data["category"] = json_data["category"].s();

        // Service를 통한 제품 업데이트
        if (productService.updateProduct(id, product_data)) {
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"message", "Product updated successfully"},
                {"id", id}
            }).dump());
        } else {
            res.code = 404;
            res.set_header("Content-Type", "application/json");
            res.write(crow::json::wvalue({
                {"error", "Product not found or invalid data"}
            }).dump());
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Internal server error"}
        }).dump());
    }
    res.end();
}

template<typename Middleware>
void ProductRouter<Middleware>::deleteProduct(const crow::request& /*req*/, crow::response& res, std::string id) {
    // Service를 통한 제품 삭제
    if (productService.deleteProduct(id)) {
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"message", "Product deleted successfully"},
            {"id", id}
        }).dump());
    } else {
        res.code = 404;
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::wvalue({
            {"error", "Product not found"}
        }).dump());
    }
    res.end();
}

// 명시적 인스턴스 선언
template class ProductRouter<struct AccessLogMiddleware>;
