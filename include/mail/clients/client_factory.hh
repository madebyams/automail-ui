#pragma once
#include "mail_client.hh"

#include <memory>

#include "config/config_service.hh"

namespace mail {
    class client_factory {
    public:
        static std::unique_ptr<client> create(app::config::service& config_service);
    };
}
