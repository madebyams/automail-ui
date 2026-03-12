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
    mail_service
)