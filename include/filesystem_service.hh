#pragma once

#include <string>

class filesystem_service {
public:
    filesystem_service();
    std::string get_file_content(const std::string path);
};