#include "mail/clients/client_factory.hh"

#include <stdexcept>

#include "config/config.hh"
#include "mail/clients/imap_client.hh"
#include "mail/clients/com/com_client.hh"


using namespace mail;

std::unique_ptr<client> client_factory::create(const config::store& config_store) {
    switch (config_store.get_config().type.get_value()) {
        case config::client_type::value::IMAP: {
            return std::make_unique<imap::client>(config_store);
        }

        case config::client_type::value::COM: {
            return std::make_unique<com::client>();
        }

        default:
            throw std::runtime_error("unknown client type");
    }
}