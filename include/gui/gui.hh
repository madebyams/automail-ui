#pragma once

#include <cstdint>
#include <string>

#include <webview/webview.h>

#include "gui/gui_config.hh"
#include "filesystem_service.hh"
#include "mail/mail_service.hh"
#include "config/config_store.hh"


using mail_service = mail::mail_service;
using config_store = config::store;

namespace gui {
    class gui_t {
        config config_;
        webview::webview window_;
        config_store config_store_;
        filesystem_service filesystem_service_;
        mail_service mail_service_;
    public:
        gui_t(config config, const filesystem_service& filesystem_service, const mail_service& mail_service, const config_store& config_store);
        void display(std::string html_path);
        void display(uint32_t server_port);
    };
}