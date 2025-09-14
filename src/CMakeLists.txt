# Source files
set(SOURCES
    main.cpp
    config/config.cpp
    router/member_router.cpp
    router/product_router.cpp
    service/member_service.cpp
    service/product_service.cpp
    repository/mysql_member_repository.cpp
    repository/mysql_product_repository.cpp
)

# Header files
set(HEADERS
    config/config.h
    middleware/access_log_middleware.h
    repository/mysql_connection_pool.h
    repository/mysql_member_repository.h
    repository/mysql_product_repository.h
    router/member_router.h
    router/product_router.h
    service/member_service.h
    service/product_service.h
    utils/benchmark.h
)

# Create library from source files
add_library(crow_ex1_lib STATIC ${SOURCES} ${HEADERS})

# Set target properties
set_target_properties(crow_ex1_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add include directories
target_include_directories(crow_ex1_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/middleware
    ${CMAKE_CURRENT_SOURCE_DIR}/repository
    ${CMAKE_CURRENT_SOURCE_DIR}/router
    ${CMAKE_CURRENT_SOURCE_DIR}/service
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/Crow/include
    ${MYSQL_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)

# Link with Crow and other dependencies
target_link_libraries(crow_ex1_lib PUBLIC 
    Crow::Crow
    Threads::Threads
    ${YAML_CPP_LIBRARIES}
    ${MYSQL_LIBRARY}
)

# Add compile definitions
target_compile_definitions(crow_ex1_lib PRIVATE
    ${MYSQL_CFLAGS_OTHER}
    ${YAML_CPP_CFLAGS_OTHER}
)

# Add warnings and optimizations
add_warnings_optimizations(crow_ex1_lib)

# Install headers
install(FILES ${HEADERS}
    DESTINATION include/crow_ex1
)
