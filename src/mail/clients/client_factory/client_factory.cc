#include "mail/clients/client_factory.hh"

#include <stdexcept>

#include "config/config.hh"
#include "mail/clients/imap_client.hh"
#ifdef _WIN32
#include "mail/clients/com/com_client.hh"
#endif


using namespace mail;

std::unique_ptr<client> client_factory::create(const app::config::store& config_store) {
    switch (config_store.get_config().type.get_value()) {
        case app::config::client_type::value::IMAP: {
            return std::make_unique<imap::client>(config_store);
        }

#ifdef _WIN32
        case app::config::client_type::value::COM: {
            return std::make_unique<com::client>();
        }
#endif

        default:
            throw std::runtime_error("unknown client type");
    }
}