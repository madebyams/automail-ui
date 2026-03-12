#include <spdlog/spdlog.h>

#include "filesystem_service.hh"
#include "config/config_store.hh"
#include "mail/mail_service.hh"
#include "config/config_factory.hh"
#include "mail/clients/imap_client.hh"
#include "gui/gui.hh"


int main() {
    spdlog::info("Starting automail ui...");

    filesystem_service fs;

    config::store cs(config::factory::from_yaml("config.yml"), fs);
    mail::imap::client client(cs);

    mail_service mail_service_(
        client
    );

    gui::gui_t gui({
        .width = 600,
        .height = 600,
        .title = "Automail"
    }, fs, mail_service_, cs);

    gui.display(4200);

    spdlog::info("Exiting automail...");
    return 0;
}