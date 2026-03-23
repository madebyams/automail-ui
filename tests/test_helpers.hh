#pragma once

#include <list>
#include <string>

#include "config/config.hh"
#include "mail/mail_service.hh"
#include "test_framework.hh"

class fake_client final : public mail::client {
public:
    void init() override {
        init_called = true;
    }

    int delete_from_senders(std::list<std::string> senders) override {
        deleted_senders = std::move(senders);
        return delete_result;
    }

    void reload() override {}

    bool init_called = false;
    int delete_result = 0;
    std::list<std::string> deleted_senders;
};

inline app::config::config_t sample_config() {
    return {
        .id = 7,
        .hostname = "imap.example.com",
        .port = 993,
        .ssl = true,
        .mailbox = "Archive",
        .type = app::config::client_type::IMAP(),
        .credential = {
            .id = 11,
            .username = "alice",
            .password = "secret"
        },
        .senders_to_be_deleted = {"one@example.com", "two@example.com"}
    };
}
