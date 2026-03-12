add_library(mail_service
    mail_service.cc
)

target_link_libraries(mail_service PUBLIC
    nlohmann_json::nlohmann_json
    mailio::mailio
    spdlog::spdlog
)