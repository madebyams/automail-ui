#include "mail/config_store.hh"

using namespace mail;

config_store::config_store(config config_, filesystem_service filesystem_service)
    : config_(std::move(config_)), filesystem_service_(std::move(filesystem_service)) {}

config config_store::get_config() const {
    return config_;
}

void config_store::save_config(const config config_) {
    this->config_ = config_;
}
