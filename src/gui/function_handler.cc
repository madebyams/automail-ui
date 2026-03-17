#include "gui/function_handler.hh"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "mail/clients/client_factory.hh"
#include "config/config_factory.hh"

using namespace gui;
using json = nlohmann::json;

function_handler::function_handler(webview::webview& window, mail::mail_service& mail_service, app::config::service& config_store)
    : window_(window),
    mail_service_(mail_service),
    config_service_(config_store) {


    this->window_.bind("doDelete", [&](const std::string& request){
        return this->do_delete(request);
    });
    this->window_.bind("hasConfig", [&](const std::string& request) {
        return this->has_config(request);
    });
    this->window_.bind("saveImapConfig", [&](const std::string& request) {
        return this->save_imap_config(request);
    });
    this->window_.bind("loadImapConfig", [&](const std::string& request) {
        return this->load_imap_config(request);
    });
    this->window_.bind("getSenders", [&](const std::string& request) {
        return this->get_senders(request);
    });
    this->window_.bind("addToSenderList", [&](const std::string& request) {
        return this->add_to_sender_list(request);
    });
    this->window_.bind("removeFromSenderList", [&](const std::string& request) {
        return this->remove_from_sender_list(request);
    });
}

std::string function_handler::get_senders(const std::string& _request) {
    return json(this->mail_service_.get_senders()).dump();
}

std::string function_handler::has_config(const std::string& _request) {
    return this->config_service_.has_config() ? "true" : "false";
}

std::string function_handler::do_delete(const std::string& request) {
    json request_object = json::parse<>(request);
    this->mail_service_.delete_from_senders(request_object[0]["senderList"]);

    return "{}";
}

std::string function_handler::load_imap_config(const std::string& _request) {
    return this->config_service_.get_config().to_json();
}

std::string function_handler::save_imap_config(const std::string& request) {
    try {
        spdlog::debug("Received request: {}", request);
        json config = json::parse<>(request);
        app::config::config_t mail_config = app::config::factory::from_json(config[0]);
        this->config_service_.save_config(mail_config);
        this->mail_service_.set_client(mail::client_factory::create(this->config_service_));
        this->mail_service_.get_client().reload();

        return "{}";
    } catch (std::exception& e) {
        spdlog::error("Error while saving config: {}", e.what());
        return "{}";
    }
}

std::string function_handler::remove_from_sender_list(const std::string& request) {
    json request_object = json::parse<>(request);
    this->mail_service_.remove_sender(request_object[0]);
    return json(this->mail_service_.get_senders()).dump();
}

std::string function_handler::add_to_sender_list(const std::string& request) {
    spdlog::debug("Received request: {}", request);
    json request_object = json::parse<>(request);
    this->mail_service_.add_sender(request_object[0]);

    json sender_json = this->mail_service_.get_senders();
    std::string jsoned = sender_json.dump();
    spdlog::debug("Result: {}", jsoned);
    return jsoned;
}