#pragma once

#include <nlohmann/json.hpp>

#include "config.hh"

using json = nlohmann::json;

namespace config {
    class factory {
    public:
        static config_t from_env();
        static config_t from_yaml(std::string path);
        static config_t from_json(json json);
    };

}