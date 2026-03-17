#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "config/config.hh"

namespace app::config {
    class service {
        constexpr static std::string_view DB_PATH = "config.db";
        SQLite::Database& db_;
        bool config_exists = false;

        credential_t get_credentials(int64_t credential_id) const;
        void save_credentials(const credential_t& credential);
    public:
        service(SQLite::Database& db_);
        bool has_config();
        config_t get_config();
        void save_config(const config_t& config);
    };
}