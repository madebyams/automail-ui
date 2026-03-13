#include <spdlog/spdlog.h>

#include "frontend_server.hh"
#include "filesystem_service.hh"
#include "config/config_store.hh"
#include "mail/mail_service.hh"
#include "config/config_factory.hh"
#include "gui/gui.hh"


int main() {
    spdlog::info("Starting automail ui...");

    frontend_server frontend_server("frontend/dist/frontend/browser");
    const filesystem_service fs;
    const app::config::store cs(app::config::factory::from_yaml("config.yml"), fs);
    mail::mail_service mail_service_(
        cs
    );
    gui::gui_t gui({
        .width = 600,
        .height = 600,
        .title = "Automail"
    }, fs, cs, mail_service_);

    frontend_server.run();
    gui.display(frontend_server.get_port());

    spdlog::info("Exiting automail...");
    return 0;
}