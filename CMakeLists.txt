cmake_minimum_required(VERSION 3.15)
project(EventManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)

add_executable(EventManagement src/main.cpp)
target_link_libraries(EventManagement PRIVATE nlohmann_json::nlohmann_json httplib::httplib)

# Copy static files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/public DESTINATION ${CMAKE_BINARY_DIR}) 