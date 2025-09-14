# Test sources
set(TEST_SOURCES
    unit/functional_test.cpp
    unit/access_log_test.cpp
    unit/test_access_prefix.cpp
    unit/test_iso8601.cpp
    unit/test_kst.cpp
    unit/unit_test_example.cpp
    unit/integration_test.cpp
)

# Test headers
set(TEST_HEADERS
    unit/test_helper.h
)

# Create test executable
add_executable(functional_test ${TEST_SOURCES} ${TEST_HEADERS})

# Add warnings and optimizations
add_warnings_optimizations(functional_test)

# Link with libraries
target_link_libraries(functional_test 
    PRIVATE 
        crow_ex1_lib
        Crow::Crow
        Threads::Threads
)

# Add include directories
target_include_directories(functional_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unit
    ${CMAKE_SOURCE_DIR}/src
)

# Add test to CTest
add_test(NAME functional_test COMMAND functional_test)

# Set test properties
set_tests_properties(functional_test PROPERTIES
    TIMEOUT 30
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
