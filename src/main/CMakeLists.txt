add_executable(main
    main.cpp
)

target_link_libraries(main
    config_factory
    filesystem_service
    mail_service
    ui

    spdlog::spdlog
)