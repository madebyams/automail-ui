#include <fstream>

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

#include "config/config_factory.hh"
#include "test_framework.hh"

namespace {
    TEST_CASE(config_factory_reads_json) {
        const nlohmann::json input = {
            {"hostname", "mail.example.com"},
            {"port", 143},
            {"ssl", false},
            {"mailbox", "INBOX"},
            {"client_type", "com"},
            {"username", "bob"},
            {"password", "hunter2"}
        };

        const auto config = app::config::factory::from_json(input);

        REQUIRE_EQ(config.hostname, "mail.example.com");
        REQUIRE_EQ(config.port, 143U);
        REQUIRE_EQ(config.ssl, false);
        REQUIRE_EQ(config.mailbox, "INBOX");
        REQUIRE_EQ(config.type.to_string(), "com");
        REQUIRE_EQ(config.credential.username, "bob");
        REQUIRE_EQ(config.credential.password, "hunter2");
    }

    TEST_CASE(config_factory_reads_yaml_file) {
        temp_dir dir;
        const auto config_path = dir.path() / "config.yml";

        std::ofstream out(config_path);
        out << "client_type: imap\n";
        out << "imap:\n";
        out << "  hostname: yaml.example.com\n";
        out << "  port: 993\n";
        out << "  ssl: true\n";
        out << "  mailbox: Inbox\n";
        out << "  username: carol\n";
        out << "  password: pass123\n";
        out.close();

        const auto config = app::config::factory::from_yaml(config_path.string());

        REQUIRE_EQ(config.hostname, "yaml.example.com");
        REQUIRE_EQ(config.port, 993U);
        REQUIRE_EQ(config.ssl, true);
        REQUIRE_EQ(config.mailbox, "Inbox");
        REQUIRE_EQ(config.type.to_string(), "imap");
        REQUIRE_EQ(config.credential.username, "carol");
        REQUIRE_EQ(config.credential.password, "pass123");
    }

    TEST_CASE(config_factory_returns_default_for_missing_yaml_file) {
        temp_dir dir;
        const auto missing_path = dir.path() / "missing.yml";

        const auto config = app::config::factory::from_yaml(missing_path.string());

        REQUIRE_EQ(config.id, -1);
        REQUIRE_EQ(config.hostname, "");
        REQUIRE_EQ(config.port, 0U);
        REQUIRE_EQ(config.ssl, true);
        REQUIRE_EQ(config.mailbox, "INBOX");
        REQUIRE_EQ(config.type.to_string(), "imap");
        REQUIRE_EQ(config.credential.username, "");
        REQUIRE_EQ(config.credential.password, "");
    }

    TEST_CASE(config_factory_reads_environment) {
        set_env("HOST", "env.example.com");
        set_env("PORT", "2525");
        set_env("USERNAME", "dave");
        set_env("PASSWORD", "pw");

        const auto config = app::config::factory::from_env();

        REQUIRE_EQ(config.hostname, "env.example.com");
        REQUIRE_EQ(config.port, 2525U);
        REQUIRE_EQ(config.credential.username, "dave");
        REQUIRE_EQ(config.credential.password, "pw");
    }
}
