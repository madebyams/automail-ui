if(WIN32)
    # On Windows assets are linked via resources.rc added to the main executable.
    add_library(frontend_server
        frontend_server.cc
    )
else()
    # On Linux the byte arrays live in a generated .cc compiled into this library.
    add_library(frontend_server
        frontend_server.cc
        "${CMAKE_BINARY_DIR}/generated/resources_data.cc"
    )
endif()

target_link_libraries(frontend_server
    httplib::httplib
    spdlog::spdlog
)