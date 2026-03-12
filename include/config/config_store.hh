#pragma once

#include "config.hh"
#include "filesystem_service.hh"

namespace config {
    class store {
        config_t config_;
        filesystem_service filesystem_service_;
    public:
        store(config_t config_, filesystem_service filesystem_service);

        config_t get_config() const;
        void save_config(config_t config_);
    };
}