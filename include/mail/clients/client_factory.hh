#pragma once
#include "mail_client.hh"

#include <memory>

#include "config/config_store.hh"

namespace mail {
    class client_factory {
    public:
        static std::unique_ptr<client> create(const config::store& config_store);
    };
}
