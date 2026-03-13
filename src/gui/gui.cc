#include "gui/gui.hh"

#include "config/config.hh"
#include "config/config_factory.hh"

#include "mail/clients/client_factory.hh"

#include <nlohmann/json.hpp>
#include <utility>
#include <spdlog/spdlog.h>

using app_config = config::config_t;
using config_factory = config::factory;

using namespace gui;
using json = nlohmann::json;

gui_t::gui_t(config config, const filesystem_service& filesystem_service, mail_service& mail_service, const config_store& config_store)
    : config_(std::move(config)),
    filesystem_service_(filesystem_service),
    mail_service_(mail_service),
    config_store_(config_store),
    window_(false, nullptr) {
    spdlog::info("Initializing UI...");

    this->window_.set_title(config_.title);
    this->window_.set_size(config_.width, config_.height, WEBVIEW_HINT_NONE);

    this->window_.bind("doDelete", [&](const std::string& request){
        json request_object = json::parse<>(request);
        this->mail_service_.delete_from_senders(request_object[0]["senderList"]);

        return "{}";
    });

    this->window_.bind("saveImapConfig", [&](const std::string& request) {
        try {
            spdlog::debug("Received request: {}", request);
            json config = json::parse<>(request);
            app_config mail_config = config_factory::from_json(config[0]);
            this->config_store_.save_config(mail_config);
            this->mail_service_.set_client(mail::client_factory::create(this->config_store_));
            this->mail_service_.get_client().reload();

            return "{}";
        } catch (std::exception& e) {
            spdlog::error("Error while saving config: {}", e.what());
            return "{}";
        }
    });

    this->window_.bind("loadImapConfig", [&](const std::string& request) {
        return this->config_store_.get_config().to_json();
    });
}


void gui_t::display(std::string html_path) {
    spdlog::info("Displaying {}...", html_path);
    std::string html = filesystem_service_.get_file_content(html_path);

    this->window_.set_html(html);
    this->window_.run();
}

void gui_t::display(uint32_t server_port) {
    spdlog::info("Displaying at :{}...", server_port);

    this->window_.navigate("http://localhost:4200");
    this->window_.run();
}