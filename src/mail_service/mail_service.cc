#include "../../include/mail/mail_service.hh"

#include <cstdlib>

#include <spdlog/spdlog.h>
#include <mailio/imap.hpp>
#include <utility>

using search_condition = mailio::imaps::search_condition_t;
using namespace mail;

mail_service::mail_service(const config_store& config_store)
    : config_store_(config_store) {
    spdlog::info("Initializing Mail Service...");
    this->cfg_ = config_store_.get_config();
}

int mail_service::delete_from_senders(const std::list<std::string>& senders) const {
    spdlog::info("Initializing mail services...");

    int deleted = 0;
    mailio::imaps client{cfg_.hostname, cfg_.port};

    client.ssl_options({
        boost::asio::ssl::context::tls_client,
        boost::asio::ssl::verify_none
    });

    spdlog::info("Mail service initialized, authenticating...");
    client.authenticate(cfg_.username, cfg_.password, mailio::imaps::auth_method_t::START_TLS);

    for (const std::string& sender : senders) {
        std::list<unsigned long> uids;
        spdlog::info("Deleting mails from {}", sender);
        client.select(cfg_.mailbox);
        client.search({search_condition{search_condition::FROM, sender}}, uids, /*want_uids=*/true);

        if (uids.empty()) {
            spdlog::debug("No messages from sender found!");
            continue;
        }

        for (unsigned long uid : uids) {
            client.remove(cfg_.mailbox, uid, /*is_uid=*/true);
            spdlog::info("Deleted message {}", uid);
            ++deleted;
        }
    }

    return deleted;
}

void mail_service::reload_config() {
    this->cfg_ = this->config_store_.get_config();
    spdlog::info("Reloaded config");
}
