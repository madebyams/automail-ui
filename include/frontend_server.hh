#pragma once

#include <cstdint>
#include <string>
#include <thread>

#include <httplib.h>

class frontend_server {
    uint32_t port;
    httplib::Server server;
    std::thread server_thread;
public:
    explicit frontend_server(std::string dist_path);
    void run();
    uint32_t get_port();
    ~frontend_server();
};