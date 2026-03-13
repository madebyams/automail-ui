#include "mail/mail_service.hh"

#include <spdlog/spdlog.h>
#include <mailio/imap.hpp>

#include "mail/clients/client_factory.hh"

using search_condition = mailio::imaps::search_condition_t;
using namespace mail;

mail_service::mail_service(const config::store& config_store)
    : client_(client_factory::create(config_store))
{}

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
