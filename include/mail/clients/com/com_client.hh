#pragma once

#ifndef WIN32
    #error("The COM client cannot be used in non Windows systems!")
#endif

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <oaidl.h>

#include "mail/clients/mail_client.hh"

namespace mail::com {
    namespace Properties {
        constexpr static wchar_t SESSION[] = L"Session";
        constexpr static wchar_t ITEMS[] = L"Items";
        constexpr static wchar_t COUNT[] = L"Count";
        constexpr static wchar_t SENDER_EMAIL_ADDRESS[] = L"SenderEmailAddress";
        constexpr static wchar_t SUBJECT[] = L"Subject";
    }
    namespace Method {
        constexpr static wchar_t GET_NAMESPACE[] = L"GetNamespace";
        constexpr static wchar_t GET_DEFAULT_FOLDER[] = L"GetDefaultFolder";
        constexpr static wchar_t MAIL_ITEM[] = L"Item";
        constexpr static wchar_t DELETE_MAIL[] = L"Delete";
    }

    class client : public mail::client {
        constexpr static wchar_t APP_NAME[] = L"Outlook.Application";

        CLSID outlook_clsid;
        std::vector<IDispatch*> dispatch_container;
        IDispatch* outlook_app = nullptr;

        bool initialized = false;
        bool instanciated = false;

        void create_instance();
        IDispatch* get_mailbox_dispatch();
        int get_mail_count(IDispatch* mailbox);
        void delete_mails_from(std::wstring sender);
    public:
        ~client() override;
        void init() override;
        int delete_from_senders(std::list<std::string> senders) override;
        void reload() override;
    };
}