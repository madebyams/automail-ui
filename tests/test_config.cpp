#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

#include "test_helpers.hh"

namespace {
    TEST_CASE(config_serializes_to_json) {
        const auto config = sample_config();

        const auto parsed = nlohmann::json::parse(config.to_json());

        REQUIRE_EQ(parsed.at("hostname").get<std::string>(), "imap.example.com");
        REQUIRE_EQ(parsed.at("port").get<uint32_t>(), 993U);
        REQUIRE_EQ(parsed.at("username").get<std::string>(), "alice");
        REQUIRE_EQ(parsed.at("password").get<std::string>(), "secret");
        REQUIRE_EQ(parsed.at("ssl").get<bool>(), true);
        REQUIRE_EQ(parsed.at("mailbox").get<std::string>(), "Archive");
        REQUIRE_EQ(parsed.at("client_type").get<std::string>(), "imap");
    }

    TEST_CASE(config_serializes_to_yaml) {
        const auto config = sample_config();

        const auto parsed = YAML::Load(config.to_yaml());

        REQUIRE_EQ(parsed["client_type"].as<std::string>(), "imap");
        REQUIRE_EQ(parsed["imap"]["hostname"].as<std::string>(), "imap.example.com");
        REQUIRE_EQ(parsed["imap"]["port"].as<uint32_t>(), 993U);
        REQUIRE_EQ(parsed["imap"]["username"].as<std::string>(), "alice");
        REQUIRE_EQ(parsed["imap"]["password"].as<std::string>(), "secret");
        REQUIRE_EQ(parsed["imap"]["ssl"].as<bool>(), true);
        REQUIRE_EQ(parsed["imap"]["mailbox"].as<std::string>(), "Archive");
    }
}
