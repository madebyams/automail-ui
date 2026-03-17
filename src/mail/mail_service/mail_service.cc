#include "mail/mail_service.hh"

#include <spdlog/spdlog.h>
#include <mailio/imap.hpp>

#include "mail/clients/client_factory.hh"

using search_condition = mailio::imaps::search_condition_t;
using namespace mail;

mail_service::mail_service(app::config::service& config_store, SQLite::Database& db)
    : client_(client_factory::create(config_store)), db_(db) {
    spdlog::info("Initializing sender_blocklist...");

    try {
        db_.exec(R"(
            CREATE TABLE IF NOT EXISTS sender_blocklist (
                id    INTEGER PRIMARY KEY,
                email TEXT    NOT NULL UNIQUE
            )
        )");

        spdlog::info("Successfully initialized sender_blocklist table!");
    } catch (const SQLite::Exception& exception) {
        spdlog::error("Error initializing sender_blocklist table: {}", exception.what());
    }
}

int mail_service::delete_from_senders(const std::list<std::string>& senders) const {
    this->client_->init();
    return this->client_->delete_from_senders(senders);
}

client& mail_service::get_client() const {
    return *this->client_;
}

void mail_service::set_client(std::unique_ptr<client> client) {
    this->client_ = std::move(client);
}

void mail_service::remove_sender(const std::string& sender) {
    SQLite::Statement statement(db_, "DELETE FROM sender_blocklist WHERE email = ?");
    statement.bind(1, sender);
    statement.exec();
}

void mail_service::add_sender(std::string sender) {
    SQLite::Statement statement(db_, R"(
        INSERT INTO sender_blocklist (email)
        VALUES (?)
    )");
    statement.bind(1, sender);
    statement.exec();
}

std::list<std::string> mail_service::get_senders() {
    this->senders.clear();

    SQLite::Statement statement(db_, R"(
        SELECT email
        FROM sender_blocklist
        ORDER BY email
    )");

    while (statement.executeStep()) {
        senders.push_back(statement.getColumn("email").getString());
    }

    return senders;
}