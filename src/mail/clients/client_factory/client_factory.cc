#include "mail/clients/client_factory.hh"

#include <stdexcept>

#include "mail/clients/imap_client.hh"
#ifdef _WIN32
#include "mail/clients/com/com_client.hh"
#endif


using namespace mail;

std::unique_ptr<client> client_factory::create(app::config::service& config_service) {
    if (!config_service.has_config()) {
        return nullptr;
    }
    const auto config = config_service.get_config();
    switch (config.type.get_value()) {
        case app::config::client_type::value::IMAP: {
            return std::make_unique<imap::client>(config_service);
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