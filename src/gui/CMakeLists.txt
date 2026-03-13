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