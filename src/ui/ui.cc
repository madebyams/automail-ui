#include "ui.hh"

#include "../../include/mail/config_factory.hh"

#include <nlohmann/json.hpp>
#include <utility>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

UI::UI(config config, const filesystem_service& filesystem_service, const mail_service& mail_service, const mail::config_store config_store)
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
        json config = json::parse<>(request);
        mail::config mail_config = mail::config_factory::from_json(config);
        this->config_store_.save_config(mail_config);
        this->mail_service_.reload_config();

        return "{}";
    });

    this->window_.bind("loadImapConfig", [&](const std::string& request) {
        return this->config_store_.get_config().to_json();
    });
}


void UI::display(std::string html_path) {
    spdlog::info("Displaying {}...", html_path);
    std::string html = filesystem_service_.get_file_content(html_path);

    this->window_.set_html(html);
    this->window_.run();
}

void UI::display(uint32_t server_port) {
    spdlog::info("Displaying at :{}...", server_port);

    this->window_.navigate("http://localhost:4200");
    this->window_.run();
}