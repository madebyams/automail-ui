#include <spdlog/spdlog.h>

#include "filesystem_service.hh"
#include "mail/config_store.hh"
#include "mail/mail_service.hh"
#include "mail/config_factory.hh"
#include "ui.hh"

using config_store = mail::config_store;
using config_factory = mail::config_factory;

int main() {
    spdlog::info("Starting automail ui...");

    filesystem_service fs;
    config_store cs(config_factory::from_yaml("config.yml"), fs);
    mail_service mail_service_(
        cs
    );

    UI ui({
        .width = 600,
        .height = 600,
        .title = "Automail"
    }, fs, mail_service_, cs);

    ui.display(4200);

    spdlog::info("Exiting automail...");
    return 0;
}