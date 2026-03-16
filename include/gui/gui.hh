#pragma once

#include <cstdint>
#include <string>

#include <webview/webview.h>

#include "gui/gui_config.hh"
#include "gui/function_handler.hh"

#include "config/config_service.hh"
#include "mail/mail_service.hh"

#include "filesystem_service.hh"


namespace gui {
    class gui_t {
        config config_;
        webview::webview window_;
        filesystem_service filesystem_service_;
        function_handler function_handler_;
    public:
        gui_t(config config, const filesystem_service& filesystem_service, app::config::service& config_service, mail::mail_service& mail_service);
        void display(std::string html_path);
        void display(uint32_t server_port);
    };
}