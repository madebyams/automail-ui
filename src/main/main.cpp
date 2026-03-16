#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "config/config_service.hh"
#include "frontend_server.hh"
#include "filesystem_service.hh"
#include "config/config_store.hh"
#include "mail/mail_service.hh"
#include "config/config_factory.hh"
#include "gui/gui.hh"


int main() {
#ifdef NDEBUG
    auto logger = spdlog::basic_logger_mt("automail", "automail.log");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
#else
    auto logger = spdlog::stdout_color_mt("automail");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
#endif

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