add_library(function_handler
    function_handler.cc
)

target_link_libraries(function_handler
    webview::core
    nlohmann_json::nlohmann_json
    spdlog::spdlog

    config_factory
    config_store
    mail_service
)

add_library(gui
        gui.cc
)

target_link_libraries(gui PUBLIC
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    webview::core

    config_factory
    config_store
    filesystem_service
    function_handler
    mail_service
)

# On Linux the webview Conan recipe is header-only and intentionally does not
# carry GTK / WebKit include paths — it expects consumers to locate them via
# pkg-config. Add them here for every target that transitively includes webview.h.
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3      REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.1)

    target_include_directories(function_handler PRIVATE
        ${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})
    target_link_libraries(function_handler
        ${GTK3_LIBRARIES} ${WEBKIT2GTK_LIBRARIES})

    target_include_directories(gui PUBLIC
        ${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})
    target_link_libraries(gui PUBLIC
        ${GTK3_LIBRARIES} ${WEBKIT2GTK_LIBRARIES})
endif()