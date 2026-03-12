#pragma once

#include <mailio/imap.hpp>

#include "mail/clients/mail_client.hh"
#include "config/config_store.hh"


namespace mail::imap {
    class client : public mail::client {
        config::config_t config_;
        config::store config_store_;
        mailio::imaps client_;
    public:
        client(const config::store& config_store);
        void init() override;
        int delete_from_senders(std::list<std::string> senders) override;
        void reload() override;
    };
}
