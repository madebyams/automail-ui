#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace config {
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
        std::string hostname;
        uint32_t port;
        std::string username;
        std::string password;
        bool ssl = true;
        std::string mailbox{"INBOX"};
        client_type type = client_type::IMAP();

        std::string to_json() const;
        std::string to_yaml() const;
    };
}
