add_executable(backend_tests
    test_main.cpp
    test_config.cpp
    test_config_factory.cpp
    test_config_service.cpp
    test_filesystem_service.cpp
    test_mail_service.cpp
)

target_link_libraries(backend_tests PRIVATE
    config
    config_factory
    config_service
    filesystem_service
    mail_service
    SQLiteCpp
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
)

add_test(NAME backend_tests COMMAND backend_tests)
