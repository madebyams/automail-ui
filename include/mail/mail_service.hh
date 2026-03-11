//
// Created by Amsakan on 06.03.26.
//

#pragma once

#include <list>
#include <string>

#include "config.hh"
#include "config_store.hh"

namespace mail {
    class mail_service
    {
    public:
        mail_service(const config_store& config_store);
        int delete_from_senders(const std::list<std::string>& senders) const;
        void reload_config();
    private:
        config cfg_;
        config_store config_store_;
    };
}
