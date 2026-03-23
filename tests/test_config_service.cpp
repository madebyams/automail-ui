#include <SQLiteCpp/SQLiteCpp.h>

#include "config/config_service.hh"
#include "test_helpers.hh"

namespace {
    TEST_CASE(config_service_persists_and_loads_imap_config) {
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        app::config::service service(db);
        auto config = sample_config();

        REQUIRE_EQ(service.has_config(), false);

        service.save_config(config);

        REQUIRE_EQ(service.has_config(), true);

        const auto loaded = service.get_config();

        REQUIRE(loaded.id > 0);
        REQUIRE_EQ(loaded.hostname, config.hostname);
        REQUIRE_EQ(loaded.port, config.port);
        REQUIRE_EQ(loaded.ssl, config.ssl);
        REQUIRE_EQ(loaded.mailbox, config.mailbox);
        REQUIRE_EQ(loaded.type.to_string(), "imap");
        REQUIRE(loaded.credential.id > 0);
        REQUIRE_EQ(loaded.credential.username, config.credential.username);
        REQUIRE_EQ(loaded.credential.password, config.credential.password);
    }

    TEST_CASE(config_service_returns_default_when_empty) {
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        app::config::service service(db);

        const auto loaded = service.get_config();

        REQUIRE_EQ(loaded.id, -1);
        REQUIRE_EQ(loaded.hostname, "");
        REQUIRE_EQ(loaded.port, 0U);
        REQUIRE_EQ(loaded.ssl, true);
        REQUIRE_EQ(loaded.mailbox, "INBOX");
    }
}
