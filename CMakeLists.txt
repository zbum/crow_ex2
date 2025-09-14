cmake_minimum_required(VERSION 3.20)

project(crow_ex2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Crow REQUIRED)
find_package(Threads REQUIRED)
find_package(unofficial-libmysql REQUIRED)
find_package(yaml-cpp REQUIRED)

# Collect all source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Crow::Crow
    Threads::Threads
    unofficial::libmysql::libmysql
    yaml-cpp::yaml-cpp
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)