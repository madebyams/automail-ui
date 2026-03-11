#include "mail/config_factory.hh"

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <cstdlib>

using config_factory = mail::config_factory;

mail::config config_factory::from_env() {
    spdlog::info("Initializing config from env...");

    return {
        .hostname = std::getenv("HOST"),
        .port = static_cast<uint32_t>(std::stoul(std::getenv("PORT"))),
        .username = std::getenv("USERNAME"),
        .password = std::getenv("PASSWORD")
    };
}

mail::config config_factory::from_yaml(const std::string path) {
    spdlog::info("Initializing config from yaml file: {}", path);

    const YAML::Node config = YAML::LoadFile(path);

    return {
        .hostname = config["host"].as<std::string>(),
        .port = config["port"].as<uint32_t>(),
        .username = config["username"].as<std::string>(),
        .password = config["password"].as<std::string>()
    };
}

mail::config config_factory::from_json(json json) {
    return {
        .hostname = json["host"],
        .port = json["port"],
        .username = json["username"],
        .password = json["password"]
    };
}
