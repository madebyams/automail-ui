#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "config/credential.hh"

using json = nlohmann::json;

namespace app::config {
    class client_type {
    public:
        enum class value {
            IMAP = 0x00,
            COM = 0x01,
            UNKNOWN = 0xFF
        };

        static client_type IMAP() {
            return {value::IMAP};
        }

        static client_type COM() {
            return {value::COM};
        }

        static client_type UNKNOWN() {
            return {value::UNKNOWN};
        }

        static client_type from_string(std::string_view str) {
            if (str == "imap") {return IMAP();}
            if (str == "com") {return COM();}
            return UNKNOWN();
        }

        std::string to_string() const {
            switch (this->value_) {
                case value::IMAP: return "imap";
                case value::COM: return "com";
                default: return "unknown";
            }
        }

        value get_value() const {
            return this->value_;
        }

    private:
        value value_;

        client_type(value value) : value_(value) {}
    };

    struct config_t {
        int64_t id = -1;
        std::string hostname;
        uint32_t port;
        bool ssl = true;
        std::string mailbox{"INBOX"};
        client_type type = client_type::IMAP();

        credential_t credential;
        std::vector<std::string> senders_to_be_deleted;

        [[nodiscard]] std::string to_json() const;
        [[nodiscard]] std::string to_yaml() const;
    };
}
