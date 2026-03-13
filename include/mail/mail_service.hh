//
// Created by Amsakan on 06.03.26.
//

#pragma once

#include <list>
#include <memory>
#include <string>

#include "mail/clients/mail_client.hh"
#include "config/config_store.hh"

namespace mail {
    class mail_service
    {
    public:
        mail_service(const app::config::store& config_store);
        int delete_from_senders(const std::list<std::string>& senders) const;
        client& get_client() const;
        void set_client(std::unique_ptr<client> client);
    private:
        std::unique_ptr<client> client_;
    };
}
