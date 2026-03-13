#include "config/config_factory.hh"

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifdef __unix__
    #include <cstdlib>
#endif

#include "config/config.hh"

using namespace config;

config_t factory::from_env() {
    spdlog::info("Initializing config from env...");

    return {
        .hostname = std::getenv("HOST"),
        .port = static_cast<uint32_t>(std::stoul(std::getenv("PORT"))),
        .username = std::getenv("USERNAME"),
        .password = std::getenv("PASSWORD")
    };
}

config_t factory::from_yaml(const std::string& path) {
    spdlog::info("Initializing config from yaml file: {}", path);

    try {
        const YAML::Node config = YAML::LoadFile(path);
        auto imap_config = config["imap"];

        spdlog::info("Config initialized successfully.");
        return {
            .hostname = imap_config["hostname"].as<std::string>(),
            .port = imap_config["port"].as<uint32_t>(),
            .username = imap_config["username"].as<std::string>(),
            .password = imap_config["password"].as<std::string>(),
            .ssl = imap_config["ssl"].as<bool>(),
            .mailbox = imap_config["mailbox"].as<std::string>(),
            .type = client_type::from_string(config["client_type"].as<std::string>())
        };
    } catch (std::runtime_error &e) {
        spdlog::error("Error when configuring: {}", e.what());
        return {};
    }
}

config_t factory::from_json(const json& json) {
    return {
        .hostname = json["hostname"],
        .port = json["port"],
        .username = json["username"],
        .password = json["password"],
        .ssl = json["ssl"],
        .mailbox = json["mailbox"],
        .type = client_type::from_string(json["client_type"])
    };
}
