#include "config/config_service.hh"

#include <spdlog/spdlog.h>

using namespace app::config;

service::service()
    : db_(DB_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    try {
        spdlog::info("Initializing config database...");

        db_.exec(R"(
            CREATE TABLE IF NOT EXISTS credentials (
                id       INTEGER PRIMARY KEY,
                username TEXT    NOT NULL,
                password TEXT    NOT NULL
            )
        )");

            db_.exec(R"(
            CREATE TABLE IF NOT EXISTS config (
                id             INTEGER PRIMARY KEY,
                hostname       TEXT,
                port           INTEGER,
                ssl            INTEGER DEFAULT 1,
                mailbox        TEXT    NOT NULL DEFAULT 'INBOX',
                client_type    TEXT    NOT NULL DEFAULT 'IMAP',
                credential_id  INTEGER REFERENCES credentials(id)
            )
        )");

            db_.exec(R"(
            CREATE TABLE IF NOT EXISTS sender_blocklist (
                id    INTEGER PRIMARY KEY,
                email TEXT    NOT NULL UNIQUE
            )
        )");
    } catch (const SQLite::Exception& exception) {
        spdlog::error("Error initializing config database: {}", exception.what());
    }
}

bool service::has_config() {
    SQLite::Statement q(db_, "SELECT COUNT(*) FROM config");
    q.executeStep();
    return q.getColumn(0).getInt() > 0;
}

config_t service::get_config() {
    SQLite::Statement config_query(db_, R"(
        select * from config
    )");

    this->config_exists = config_query.executeStep();
    if (!this->config_exists) {
        return {};
    }

    config_t config = {
        .id = config_query.getColumn("id").getInt64(),
        .hostname = config_query.getColumn("hostname").getString(),
        .port = static_cast<uint32_t>(config_query.getColumn("port").getInt()),
        .ssl = config_query.getColumn("ssl").getInt() != 0,
        .mailbox = config_query.getColumn("mailbox").getString(),
        .type = client_type::from_string(config_query.getColumn("client_type").getString()),
    };

    if (config.type.get_value() == client_type::value::IMAP) {
        config.credential = get_credentials(config_query.getColumn("credential_id").getInt64());
    }

    return config;
}

credential_t service::get_credentials(const int64_t credential_id) const {
    SQLite::Statement credential_query(db_, R"(
        select * from credentials where id = ?
    )");

    credential_query.bind(1, credential_id);
    if (!credential_query.executeStep()) {
        throw std::runtime_error(
            "No credential found for id: " + std::to_string(credential_id)
        );
    }

    return {
        .id = credential_query.getColumn("id").getInt64(),
        .username = credential_query.getColumn("username").getString(),
        .password = credential_query.getColumn("password").getString()
    };
}

void service::save_config(const config_t& config) {
    try {
        db_.exec("DELETE FROM config");
        db_.exec("DELETE FROM credentials");

        save_credentials(config.credential);
        const int64_t credential_id = db_.getLastInsertRowid();

        SQLite::Statement insert_statement(db_, R"(
            insert into config (
                hostname,
                port,
                ssl,
                mailbox,
                client_type,
                credential_id
            ) values (
                ?, ?, ?, ?, ?, ?
            )
        )");
        insert_statement.bind(1, config.hostname);
        insert_statement.bind(2, config.port);
        insert_statement.bind(3, config.ssl);
        insert_statement.bind(4, config.mailbox);
        insert_statement.bind(5, config.type.to_string());
        insert_statement.bind(6, credential_id);
        insert_statement.exec();
    } catch (const SQLite::Exception& e) {
        spdlog::error("Error saving config: {}", e.what());
    }
}

void service::save_credentials(const credential_t& credential) {
    SQLite::Statement credential_statement(db_, R"(
            insert into credentials (
                username,
                password
            ) values (
                ?, ?
            )
        )");
    credential_statement.bind(1, credential.username);
    credential_statement.bind(2, credential.password);
    credential_statement.exec();
}
