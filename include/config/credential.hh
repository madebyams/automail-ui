#pragma once

#include <cstdint>
#include <string>

namespace app::config {
    struct credential_t {
        int64_t id = -1;
        std::string username;
        std::string password;
    };
}