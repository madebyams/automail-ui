#include "mail/mail_service.hh"


#include <spdlog/spdlog.h>
#include <mailio/imap.hpp>

using search_condition = mailio::imaps::search_condition_t;
using namespace mail;

mail_service::mail_service(client& client)
    : client_(client)
{}

int mail_service::delete_from_senders(const std::list<std::string>& senders) const {
    this->client_.init();
    return this->client_.delete_from_senders(senders);
}

client& mail_service::get_client() const {
    return this->client_;
}