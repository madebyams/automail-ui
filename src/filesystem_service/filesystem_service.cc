#include "filesystem_service.hh"

#include <filesystem>
#include <fstream>

#include <spdlog/spdlog.h>

filesystem_service::filesystem_service() {
    spdlog::info("Initalizing Filesystem...");
}

std::string filesystem_service::get_file_content(const std::string path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File not found");
    }

    spdlog::info("Opening {}...", path);
    std::ifstream file(path);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
