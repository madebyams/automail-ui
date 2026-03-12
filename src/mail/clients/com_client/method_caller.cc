#include "mail/clients/com/method_caller.hh"

#include <array>
#include <string>

method_caller::method_caller() {
    VariantInit(&result);
}

method_caller& method_caller::with_dispatch(IDispatch* dispatch) {
    this->dispatch = dispatch;
    return *this;
}

method_caller& method_caller::with_method_name(LPCOLESTR method_name) {
    this->method_name = method_name;
    return *this;
}

method_caller& method_caller::with_argument(int argument) {
    VARIANT variant_argument;
    VariantInit(&variant_argument);
    variant_argument.vt = VT_I4;
    variant_argument.lVal = argument;

    this->arguments.push_back(variant_argument);
    return *this;
}

method_caller& method_caller::with_argument(std::wstring argument) {
    VARIANT variant_argument;
    VariantInit(&variant_argument);
    variant_argument.vt = VT_BSTR;
    variant_argument.bstrVal = SysAllocString(argument.c_str());

    this->arguments.push_back(variant_argument);
    return *this;
}

method_caller& method_caller::call() {
    DISPID id;
    DISPPARAMS arguments = {
        this->arguments.data(),
        nullptr,
        static_cast<UINT>(this->arguments.size()),
        0
    };

    std::array<LPOLESTR, 1> method_names = {
        const_cast<LPOLESTR>(method_name)
    };

    dispatch->GetIDsOfNames(
        IID_NULL,
        method_names.data(),
        method_names.size(),
        LOCALE_USER_DEFAULT,
        &id
    );

    dispatch->Invoke(
        id,
        IID_NULL,
        LOCALE_USER_DEFAULT,
        DISPATCH_METHOD | DISPATCH_PROPERTYGET,
        &arguments,
        &this->result,
        nullptr,
        nullptr
    );
    return *this;
}

void method_caller::run() {
    DISPID id;
    DISPPARAMS arguments = {
        this->arguments.data(),
        nullptr,
        static_cast<UINT>(this->arguments.size()),
        0
    };
    std::array method_names = {
        const_cast<LPOLESTR>(method_name)
    };

    dispatch->GetIDsOfNames(
        IID_NULL,
        method_names.data(),
        method_names.size(),
        LOCALE_USER_DEFAULT,
        &id
    );

    dispatch->Invoke(
        id,
        IID_NULL,
        LOCALE_USER_DEFAULT,
        DISPATCH_METHOD,
        &arguments,
        nullptr,
        nullptr,
        nullptr
    );
}

IDispatch* method_caller::as_dispatch() {
    return this->result.vt == VT_DISPATCH ? result.pdispVal : nullptr;
}