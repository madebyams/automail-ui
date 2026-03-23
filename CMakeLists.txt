cmake_minimum_required(VERSION 3.25)
project(automail_ui VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0601)  # Windows 7+ (avoids GetAddrInfoExCancel in old MinGW headers)
endif()

# Set to OFF to build only the backend libraries and tests (no GUI, no frontend dist required).
option(BUILD_MAIN_APP "Build the main GUI application" ON)

if(BUILD_MAIN_APP)
    # Embed the Angular dist bundle into the binary on all platforms.
    # Windows → RCDATA resources; Linux → compiled-in C++ byte arrays.
    # Re-run CMake after every `ng build` to regenerate with new hashed filenames.
    set(DIST_DIR      "${CMAKE_SOURCE_DIR}/frontend/dist/frontend/browser")
    set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
    file(MAKE_DIRECTORY "${GENERATED_DIR}")

    # Glob the dist files so CMake reconfigures automatically when they change
    # (e.g. after a new `ng build` produces new hashed filenames).
    file(GLOB_RECURSE _dist_files "${DIST_DIR}/*")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_dist_files})

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -DDIST_DIR=${DIST_DIR}
            -DRC_OUTPUT=${GENERATED_DIR}/resources.rc
            -DMAP_OUTPUT=${GENERATED_DIR}/resources_map.hh
            -DCC_OUTPUT=${GENERATED_DIR}/resources_data.cc
            -DCHH_OUTPUT=${GENERATED_DIR}/resources_data.hh
            -P ${CMAKE_SOURCE_DIR}/cmake/embed_resources.cmake
        RESULT_VARIABLE _embed_result
    )
    if(NOT _embed_result EQUAL 0)
        message(FATAL_ERROR "embed_resources.cmake failed. Run 'ng build' first.")
    endif()

    # Both platforms need the generated headers
    include_directories(${GENERATED_DIR})
endif()

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(mailio REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SQLiteCpp REQUIRED)

if(BUILD_MAIN_APP)
    find_package(webview REQUIRED)
    find_package(httplib REQUIRED)
endif()

include_directories(include)
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

