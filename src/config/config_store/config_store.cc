#include "config/config_store.hh"

using namespace config;

store::store(config_t config_, filesystem_service filesystem_service)
    : config_(std::move(config_)), filesystem_service_(std::move(filesystem_service)) {}

config_t store::get_config() const {
    return config_;
}

void store::save_config(const config_t config_) {
    this->config_ = config_;
}
