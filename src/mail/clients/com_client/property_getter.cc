//
// Created by abav on 05.03.2026.
//

#include "mail/clients/com/property_getter.hh"

#include <array>

property_getter::property_getter() {
    VariantInit(&result);
}

property_getter& property_getter::with_dispatch(IDispatch* dispatch) {
    this->dispatch = dispatch;
    return *this;
}

property_getter& property_getter::with_property_name(LPCOLESTR property_name) {
    this->property_name = property_name;
    return *this;
}

property_getter& property_getter::get() {
    DISPID id;
    DISPPARAMS arguments = {};
    std::array<LPOLESTR, 1> property_names = {
        const_cast<LPOLESTR>(property_name)
    };

    dispatch->GetIDsOfNames(
        IID_NULL,
        property_names.data(),
        property_names.size(),
        LOCALE_USER_DEFAULT,
        &id
    );

    dispatch->Invoke(
        id,
        IID_NULL,
        LOCALE_USER_DEFAULT,
        DISPATCH_PROPERTYGET,
        &arguments,
        &this->result,
        nullptr,
        nullptr
    );

    return *this;
}

IDispatch* property_getter::as_dispatch() {
    return this->result.vt == VT_DISPATCH ? result.pdispVal : nullptr;
}

int property_getter::as_int() {
    return this->result.vt == VT_I4 ? result.lVal : 0;
}

std::wstring property_getter::as_string() {
    return this->result.vt == VT_BSTR ? std::wstring(result.bstrVal) : L"";
}