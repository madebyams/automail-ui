#pragma once

#include <string>
#include <vector>

#include <Windows.h>


class method_caller {
    IDispatch* dispatch = nullptr;
    LPCOLESTR method_name = nullptr;
    std::vector<VARIANT> arguments;
    VARIANT result;

public:
    method_caller();

    method_caller& with_dispatch(IDispatch* dispatch);
    method_caller& with_method_name(LPCOLESTR method_name);
    method_caller& with_argument(int argument);
    method_caller& with_argument(std::wstring argument);

    method_caller& call();
    void run();

    IDispatch* as_dispatch();
};