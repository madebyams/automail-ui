cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES CXX)

find_package(webview REQUIRED)

add_executable(test_package WIN32 src/main.cpp)
target_link_libraries(test_package PRIVATE webview::core)
