add_library(client_factory
    client_factory.cc
)

target_link_libraries(client_factory PUBLIC
        nlohmann_json::nlohmann_json
        imap_client
        com_client
        config_store
)