#pragma once

#include "mail/mail_service.hh"

#include <webview/webview.h>

namespace gui {
    class function_handler {
        webview::webview& window_;
        mail::mail_service& mail_service_;
        app::config::service& config_service_;

        std::string do_delete(const std::string& request);
        std::string has_config(const std::string& request);
        std::string load_imap_config(const std::string& request);
        std::string save_imap_config(const std::string& request);
    public:
        function_handler(webview::webview& window, mail::mail_service& mail_service, app::config::service& config_store);
    };
}