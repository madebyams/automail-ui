add_library(config_store
        config_store.cc
)

target_link_libraries(config_store
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)