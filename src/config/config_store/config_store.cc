#include "config/config_store.hh"

#include <spdlog/spdlog.h>

using namespace app::config;

store::store(config_t config_, const filesystem_service& filesystem_service)
    : config_(std::move(config_)), filesystem_service_(filesystem_service) {}

config_t store::get_config() const {
    return config_;
}

void store::save_config(const config_t& config_) {
    try {
        spdlog::info("Saving config...");
        this->config_ = config_;
        this->filesystem_service_.store_file_content("config.yml", this->config_.to_yaml());
        spdlog::info("Saved config successfully.");
    } catch (std::exception& e) {
        spdlog::error("Error while saving config: {}", e.what());
    }
}
