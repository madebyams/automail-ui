#pragma once

#include <cstdint>
#include <string>

#include <webview/webview.h>

#include "filesystem_service.hh"
#include "mail/mail_service.hh"
#include "mail/config_store.hh"

using mail_service = mail::mail_service;

class UI {
    struct config {
        uint32_t width;
        uint32_t height;
        std::string title;
    };

    config config_;
    webview::webview window_;
    mail::config_store config_store_;
    filesystem_service filesystem_service_;
    mail_service mail_service_;
public:
    UI(config config, const filesystem_service& filesystem_service, const mail_service& mail_service, mail::config_store config_store);
    void display(std::string html_path);
    void display(uint32_t server_port);
};