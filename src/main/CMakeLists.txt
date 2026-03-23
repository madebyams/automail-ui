if(WIN32)
    add_executable(automailer_ui
        main.cpp
        "${CMAKE_BINARY_DIR}/generated/resources.rc"
    )
else()
    add_executable(automailer_ui
        main.cpp
    )
endif()

target_link_libraries(automailer_ui
    config
    config_factory
    config_service
    filesystem_service
    frontend_server
    imap_client
    mail_service
    gui

    spdlog::spdlog
    SQLiteCpp
)


if(MINGW)
    target_link_options(automailer_ui PRIVATE
        -static-libgcc
        -static-libstdc++
        -Wl,-Bstatic,-lwinpthread,-Bdynamic
    )
endif()