# Benchmark sources
set(BENCHMARK_SOURCES
    benchmark_test.cpp
)

# Create benchmark executable
add_executable(benchmark_test ${BENCHMARK_SOURCES})

# Add warnings and optimizations
add_warnings_optimizations(benchmark_test)

# Link with libraries
target_link_libraries(benchmark_test 
    PRIVATE 
        crow_ex1_lib
        Crow::Crow
        Threads::Threads
)

# Add include directories
target_include_directories(benchmark_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Add benchmark to CTest
add_test(NAME benchmark_test COMMAND benchmark_test)

# Set test properties
set_tests_properties(benchmark_test PROPERTIES
    TIMEOUT 60
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Create custom target for API performance test
add_custom_target(test_api
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_api_performance.sh
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running API performance test"
)
