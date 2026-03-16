#pragma once

#include <mailio/imap.hpp>

#include "config/config_service.hh"
#include "mail/clients/mail_client.hh"


namespace mail::imap {
    class client : public mail::client {
        app::config::service& config_service_;
        app::config::config_t config_;
        mailio::imaps client_;
    public:
        client(app::config::service& config_service);
        void init() override;
        int delete_from_senders(std::list<std::string> senders) override;
        void reload() override;
    };
}
