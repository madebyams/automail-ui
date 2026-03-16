#include "gui/gui.hh"

#include "config/config.hh"

#include <nlohmann/json.hpp>
#include <utility>
#include <spdlog/spdlog.h>


using namespace gui;
using json = nlohmann::json;

gui_t::gui_t(config config, const filesystem_service& filesystem_service, app::config::service& config_service, mail::mail_service& mail_service)
    : config_(std::move(config)),
    filesystem_service_(filesystem_service),
    window_(false, nullptr),
    function_handler_{window_, mail_service, config_service}{
    spdlog::info("Initializing UI...");

    this->window_.set_title(config_.title);
    this->window_.set_size(config_.width, config_.height, WEBVIEW_HINT_NONE);
}


void gui_t::display(std::string html_path) {
    spdlog::info("Displaying {}...", html_path);
    std::string html = filesystem_service_.get_file_content(html_path);

    this->window_.set_html(html);
    this->window_.run();
}

void gui_t::display(uint32_t server_port) {
    spdlog::info("Displaying at :{}...", server_port);

    this->window_.navigate("http://localhost:" + std::to_string(server_port));
    this->window_.run();
}

