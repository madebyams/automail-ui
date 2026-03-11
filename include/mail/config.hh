#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mail {
    struct config {
        std::string hostname;
        uint32_t port;
        std::string username;
        std::string password;
        bool ssl = true;
        std::string mailbox{"INBOX"};

        std::string to_json() const {
            return json{
                {"hostname", hostname},
                {"port", port},
                {"username", username},
                {"password", password},
                {"ssl", ssl},
                {"mailbox", mailbox}
            }.dump(2);
}
    };
}
