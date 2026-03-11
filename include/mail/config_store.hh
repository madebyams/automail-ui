#pragma once

#include "config.hh"
#include "../filesystem_service.hh"

namespace mail {
    class config_store {
        config config_;
        filesystem_service filesystem_service_;
    public:
        config_store(config config_, filesystem_service filesystem_service);

        config get_config() const;
        void save_config(const config config_);
    };
}