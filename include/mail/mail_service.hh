//
// Created by Amsakan on 06.03.26.
//

#pragma once

#include <list>
#include <string>

#include "mail/clients/mail_client.hh"

namespace mail {
    class mail_service
    {
    public:
        mail_service(client& client);
        int delete_from_senders(const std::list<std::string>& senders) const;
        client& get_client() const;
    private:
        client& client_;
    };
}
