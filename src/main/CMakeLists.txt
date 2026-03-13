add_executable(main
    main.cpp
)

target_link_libraries(main
    config
    config_factory
    filesystem_service
    imap_client
    mail_service
    gui

    spdlog::spdlog
)