cmake_minimum_required(VERSION 4.3)
project(automail_ui)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(webview REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(mailio REQUIRED)
find_package(yaml-cpp REQUIRED)

include_directories(include)
add_subdirectory(src)
