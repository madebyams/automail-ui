#include <list>
#include <memory>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>

#include "config/config_service.hh"
#include "mail/mail_service.hh"
#include "test_helpers.hh"

namespace {
    TEST_CASE(mail_service_manages_sender_blocklist) {
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        app::config::service config_service(db);
        mail::mail_service service(config_service, db);

        service.add_sender("zeta@example.com");
        service.add_sender("alpha@example.com");

        auto senders = service.get_senders();
        REQUIRE_EQ(senders.size(), static_cast<std::size_t>(2));
        REQUIRE_EQ(senders.front(), "alpha@example.com");
        REQUIRE_EQ(senders.back(), "zeta@example.com");

        service.remove_sender("alpha@example.com");
        senders = service.get_senders();

        REQUIRE_EQ(senders.size(), static_cast<std::size_t>(1));
        REQUIRE_EQ(senders.front(), "zeta@example.com");
    }

    TEST_CASE(mail_service_delegates_deletion_to_client) {
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        app::config::service config_service(db);
        mail::mail_service service(config_service, db);

        auto fake = std::make_unique<fake_client>();
        auto* fake_ptr = fake.get();
        fake_ptr->delete_result = 42;
        service.set_client(std::move(fake));

        const std::list<std::string> senders = {"a@example.com", "b@example.com"};
        const auto deleted = service.delete_from_senders(senders);

        REQUIRE_EQ(deleted, 42);
        REQUIRE_EQ(fake_ptr->init_called, true);
        REQUIRE(fake_ptr->deleted_senders == senders);
    }
}
