#include "mail/clients/imap_client.hh"

#include <spdlog/spdlog.h>

using search_condition = mailio::imaps::search_condition_t;
using namespace mail::imap;

client::client(app::config::service& config_service)
    : config_service_(config_service),
    config_(config_service_.get_config()),
    client_{config_.hostname, config_.port}
{}

void client::init() {
    this->client_.ssl_options({
        boost::asio::ssl::context::tls_client,
        boost::asio::ssl::verify_none
    });

    this->client_.authenticate(config_.credential.username, config_.credential.password, mailio::imaps::auth_method_t::START_TLS);
}

int client::delete_from_senders(std::list<std::string> senders) {
    int deleted = 0;

    for (const std::string& sender : senders) {
        std::list<unsigned long> uids;
        spdlog::info("Deleting mails from {}", sender);
        this->client_.select(this->config_.mailbox);
        this->client_.search({search_condition{search_condition::FROM, sender}}, uids, /*want_uids=*/true);

        if (uids.empty()) {
            spdlog::debug("No messages from sender found!");
            continue;
        }

        for (unsigned long uid : uids) {
            this->client_.remove(this->config_.mailbox, uid, /*is_uid=*/true);
            spdlog::info("Deleted message {}", uid);
            ++deleted;
        }
    }

    return deleted;
}

void client::reload() {
    this->config_ = this->config_service_.get_config();
    spdlog::info("Reloaded config");
}
