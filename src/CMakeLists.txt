add_subdirectory(config)
add_subdirectory(filesystem_service)
add_subdirectory(mail)

if(BUILD_MAIN_APP)
    add_subdirectory(frontend_server)
    add_subdirectory(main)
    add_subdirectory(gui)
endif()