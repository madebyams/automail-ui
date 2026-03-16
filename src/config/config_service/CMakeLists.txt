add_library(config_service
    config_service.cc
)

target_link_libraries(config_service
    SQLiteCpp
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)