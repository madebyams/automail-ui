add_library(config_factory
    config_factory.cc
)

target_link_libraries(config_factory
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    yaml-cpp::yaml-cpp
)