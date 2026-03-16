if(WIN32)
    add_executable(main
        main.cpp
        "${CMAKE_BINARY_DIR}/generated/resources.rc"
    )
else()
    add_executable(main
        main.cpp
    )
endif()

target_link_libraries(main
    config
    config_factory
    config_service
    filesystem_service
    frontend_server
    imap_client
    mail_service
    gui

    spdlog::spdlog
)