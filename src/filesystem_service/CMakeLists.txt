add_library(filesystem_service
    filesystem_service.cc
)

target_link_libraries(filesystem_service PUBLIC
    spdlog::spdlog
)