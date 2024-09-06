cmake_minimum_required(VERSION 3.13)
project(http-server-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
# Find the zlib library
find_package(ZLIB REQUIRED)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(server ${SOURCE_FILES})
target_link_libraries(server pthread)
target_link_libraries(server ZLIB::ZLIB)
