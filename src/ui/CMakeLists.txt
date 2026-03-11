add_library(ui
    ui.cc
)

target_link_libraries(ui PUBLIC
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    webview::core

    config_factory
    config_store
    filesystem_service
    mail_service
)