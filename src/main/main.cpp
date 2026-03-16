#include <spdlog/spdlog.h>

#include "config/config_service.hh"
#include "frontend_server.hh"
#include "filesystem_service.hh"
#include "config/config_store.hh"
#include "mail/mail_service.hh"
#include "config/config_factory.hh"
#include "gui/gui.hh"


int main() {
    spdlog::info("Starting automail ui...");

    app::config::service config_service;
    frontend_server frontend_server("frontend/dist/frontend/browser");
    const filesystem_service fs;

    auto config = config_service.get_config();
    spdlog::debug("Config: {}", config.to_json());

    mail::mail_service mail_service_(
        config_service
    );
    gui::gui_t gui({
        .width = 600,
        .height = 600,
        .title = "Automail"
    }, fs, config_service, mail_service_);


    frontend_server.run();
    gui.display(frontend_server.get_port());

    spdlog::info("Exiting automail...");
    return 0;
}