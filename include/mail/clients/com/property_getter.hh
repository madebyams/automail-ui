#pragma once

#include <string>
#include <Windows.h>

class property_getter {
    IDispatch* dispatch = nullptr;
    LPCOLESTR property_name = nullptr;
    VARIANT result;

public:
    property_getter();

    property_getter& with_dispatch(IDispatch* dispatch);
    property_getter& with_property_name(LPCOLESTR property_name);

    property_getter& get();

    IDispatch* as_dispatch();
    int as_int();
    std::wstring as_string();
};