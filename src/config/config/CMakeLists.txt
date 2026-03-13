add_library(config
    config.cc
)

target_link_libraries(config
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
)