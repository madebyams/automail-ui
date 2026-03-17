//
// Created by Amsakan on 06.03.26.
//

#pragma once

#include <list>
#include <memory>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>

#include "mail/clients/mail_client.hh"
#include "config/config_service.hh"

namespace mail {
    class mail_service
    {
    public:
        mail_service(app::config::service& config_service, SQLite::Database& db);
        void add_sender(std::string sender);
        void remove_sender(const std::string& sender);
        int delete_from_senders(const std::list<std::string>& senders) const;
        client& get_client() const;
        void set_client(std::unique_ptr<client> client);
        std::list<std::string> get_senders();
    private:
        SQLite::Database& db_;
        std::unique_ptr<client> client_;
        std::list<std::string> senders;
    };
}
