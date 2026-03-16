add_library(imap_client
    imap_client.cc
)

target_link_libraries(imap_client
    mailio::mailio
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    SQLiteCpp
)