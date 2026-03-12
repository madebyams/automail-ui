add_library(property_getter
    property_getter.cc
)

add_library(method_caller
    method_caller.cc
)

add_library(com_client
    com_client.cc
)

target_link_libraries(com_client
    spdlog::spdlog
    property_getter
    method_caller
)

