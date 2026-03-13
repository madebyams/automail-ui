#include "gui/function_handler.hh"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "mail/clients/client_factory.hh"
#include "config/config_factory.hh"

using namespace gui;
using json = nlohmann::json;

function_handler::function_handler(webview::webview& window, mail::mail_service& mail_service, const app::config::store& config_store)
    : window_(window),
    mail_service_(mail_service),
    config_store_(config_store) {


    this->window_.bind("doDelete", [&](const std::string& request){
    return this->do_delete(request);
    });
    this->window_.bind("saveImapConfig", [&](const std::string& request) {
        return this->save_imap_config(request);
    });
    this->window_.bind("loadImapConfig", [&](const std::string& request) {
        return this->load_imap_config(request);
    });
}

std::string function_handler::do_delete(const std::string& request) {
    json request_object = json::parse<>(request);
    this->mail_service_.delete_from_senders(request_object[0]["senderList"]);

    return "{}";
}

std::string function_handler::load_imap_config(const std::string& _request) {
    return this->config_store_.get_config().to_json();
}

std::string function_handler::save_imap_config(const std::string& request) {
    try {
        spdlog::debug("Received request: {}", request);
        json config = json::parse<>(request);
        app::config::config_t mail_config = app::config::factory::from_json(config[0]);
        this->config_store_.save_config(mail_config);
        this->mail_service_.set_client(mail::client_factory::create(this->config_store_));
        this->mail_service_.get_client().reload();

        return "{}";
    } catch (std::exception& e) {
        spdlog::error("Error while saving config: {}", e.what());
        return "{}";
    }
}
