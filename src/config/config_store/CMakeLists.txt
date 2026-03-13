add_library(config_store
        config_store.cc
)

target_link_libraries(config_store
    config
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)