#include "frontend_server.hh"

#include <spdlog/spdlog.h>
#include "resources_map.hh"    // URL→ID map + mime_type_for() – always generated

#ifdef _WIN32
// ── Windows: load assets from RCDATA embedded in the PE ──────────────────────
#include <windows.h>

static void register_resource_routes(httplib::Server& server) {
    server.Get(".*", [](const httplib::Request& req, httplib::Response& res) {
        auto it = RESOURCE_MAP.find(req.path);
        if (it == RESOURCE_MAP.end())
            it = RESOURCE_MAP.find("/");   // SPA fallback → index.html
        if (it == RESOURCE_MAP.end()) { res.status = 404; return; }

        const auto& ref = it->second;   // {id, mime} – MIME from actual filename
        HRSRC   hRes  = FindResource(nullptr, MAKEINTRESOURCE(ref.id), RT_RCDATA);
        HGLOBAL hGlob = LoadResource(nullptr, hRes);
        DWORD   size  = SizeofResource(nullptr, hRes);
        const char* data = static_cast<const char*>(LockResource(hGlob));

        res.set_content(std::string(data, size), std::string(ref.mime));
    });
}

#else
// ── Linux: load assets from compiled-in C++ byte arrays ──────────────────────
#include "resources_data.hh"   // declares RESOURCE_DATA

static void register_resource_routes(httplib::Server& server) {
    server.Get(".*", [](const httplib::Request& req, httplib::Response& res) {
        auto it = RESOURCE_MAP.find(req.path);
        if (it == RESOURCE_MAP.end())
            it = RESOURCE_MAP.find("/");   // SPA fallback → index.html
        if (it == RESOURCE_MAP.end()) { res.status = 404; return; }

        const auto& ref = it->second;   // {id, mime} – MIME from actual filename
        auto data_it = RESOURCE_DATA.find(ref.id);
        if (data_it == RESOURCE_DATA.end()) { res.status = 404; return; }

        const auto& entry = data_it->second;
        res.set_content(
            std::string(reinterpret_cast<const char*>(entry.data), entry.size),
            std::string(ref.mime));
    });
}
#endif

frontend_server::frontend_server(std::string dist_path) {
    (void)dist_path;   // assets are embedded on all platforms now
    register_resource_routes(this->server);
    this->port = this->server.bind_to_any_port("localhost");
    spdlog::info("Frontend server bound to port {}", this->port);
}

void frontend_server::run() {
    // Run in a background thread so the caller is not blocked
    server_thread = std::thread([this]() {
        this->server.listen_after_bind();
    });
}

uint32_t frontend_server::get_port() {
    return this->port;
}

frontend_server::~frontend_server() {
    this->server.stop();
    if (server_thread.joinable()) {
        server_thread.join();
    }
}