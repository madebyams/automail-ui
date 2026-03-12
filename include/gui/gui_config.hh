#pragma once

#include <cstdint>
#include <string>

namespace gui {
    struct config {
        uint32_t width;
        uint32_t height;
        std::string title;
    };
}