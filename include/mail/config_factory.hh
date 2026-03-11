#pragma once

#include <nlohmann/json.hpp>

#include "mail_service.hh"

using json = nlohmann::json;

namespace mail {
    class config_factory {
    public:
        static config from_env();
        static config from_yaml(std::string path);
        static config from_json(json json);
    };

}