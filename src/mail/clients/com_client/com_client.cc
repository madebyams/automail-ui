#include "mail/clients/com/com_client.hh"

#include <spdlog/spdlog.h>

#include "mail/clients/com/method_caller.hh"
#include "mail/clients/com/property_getter.hh"

using namespace mail::com;

client::~client() {
    for (IDispatch* dispatch : this->dispatch_container) {
        dispatch->Release();
    }

    CoUninitialize();
}

void client::init() {
    CoInitialize(nullptr);

    spdlog::info("Initializing Outlook...");

    if (auto err = CLSIDFromProgID(APP_NAME, &(this->outlook_clsid)); FAILED(err)) {
        spdlog::error("Failed to initialize Outlook: {}", err);
        return;
    }

    this->initialized = true;
    spdlog::info("Outlook initialized successfully.");
}

int client::delete_from_senders(std::list<std::string> senders) {
    int deleted = 0;

    for (std::string sender : senders) {
        spdlog::info("Deleting mails from {}", std::string(sender.begin(), sender.end()));
        std::wstring w_sender = std::wstring(sender.begin(), sender.end());
        this->delete_mails_from(w_sender);
        spdlog::info("Deleted mails from {} successfully.", std::string(sender.begin(), sender.end()));
    }

    return deleted;
}

void client::reload() {}

void client::delete_mails_from(std::wstring sender) {
    if (!this->initialized) {
        this->init();
    }

    if (!this->instanciated) {
        this->create_instance();
    }

    spdlog::info("Fetching mailbox");
    auto mailbox = this->get_mailbox_dispatch();
    int mail_count = this->get_mail_count(mailbox);
    this->dispatch_container.push_back(mailbox);

    spdlog::info("Deleting mails from {}", std::string(sender.begin(), sender.end()));
    for (int mail_index = mail_count; mail_index >= 1; mail_index--) {
        auto mail_dispatch = method_caller()
            .with_dispatch(mailbox)
            .with_method_name(Method::MAIL_ITEM)
            .with_argument(mail_index) // Mail items are 1-indexed
            .call()
            .as_dispatch();

        if (!mail_dispatch) continue;

        this->dispatch_container.push_back(mail_dispatch);
        std::wstring mail_sender = property_getter()
            .with_dispatch(mail_dispatch)
            .with_property_name(Properties::SENDER_EMAIL_ADDRESS)
            .get()
            .as_string();

        if (mail_sender == sender) {
            spdlog::info("Deleting {}. mail from {}", mail_index, std::string(sender.begin(), sender.end()));

            method_caller()
                .with_dispatch(mail_dispatch)
                .with_method_name(Method::DELETE_MAIL)
                .run();
        }
    }
}

void client::create_instance() {
    spdlog::info("Creating Outlook instance...");

    auto err = CoCreateInstance(
        this->outlook_clsid,
        nullptr,
        CLSCTX_LOCAL_SERVER,
        IID_IDispatch,
        reinterpret_cast<void**>(&this->outlook_app)
    );

    if (FAILED(err)) {
        spdlog::error("Failed to create Outlook instance: {}", err);
        return;
    }

    spdlog::info("Outlook instance created successfully.");
    this->dispatch_container.push_back(this->outlook_app);
    this->instanciated = SUCCEEDED(err);
}

IDispatch* client::get_mailbox_dispatch() {
    spdlog::info("Getting session...");
    auto session = property_getter()
        .with_dispatch(this->outlook_app)
        .with_property_name(Properties::SESSION)
        .get()
        .as_dispatch();
    this->dispatch_container.push_back(session);
    spdlog::info("Session: {}", session ? "OK" : "NULL");

    spdlog::info("Getting inbox...");
    auto inbox = method_caller()
        .with_dispatch(session)
        .with_method_name(Method::GET_DEFAULT_FOLDER)
        .with_argument(6)
        .call()
        .as_dispatch();
    this->dispatch_container.push_back(inbox);
    spdlog::info("Inbox: {}", inbox ? "OK" : "NULL");

    return property_getter()
        .with_dispatch(inbox)
        .with_property_name(Properties::ITEMS)
        .get()
        .as_dispatch();
}

int client::get_mail_count(IDispatch* mailbox) {
    return property_getter()
        .with_dispatch(mailbox)
        .with_property_name(Properties::COUNT)
        .get()
        .as_int();
}