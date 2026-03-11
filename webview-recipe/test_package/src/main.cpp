#include "webview/webview.h"

#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/,
                   LPSTR lpCmdLine, int /*nCmdShow*/) {
    const bool check_only = (lpCmdLine && std::strstr(lpCmdLine, "--check"));
#else
int main(int argc, char* argv[]) {
    bool check_only = false;
    for (int i = 1; i < argc; ++i)
        if (std::strcmp(argv[i], "--check") == 0) check_only = true;
#endif

    // Verify the header is usable and the version macro is accessible.
    std::cout << "webview version: "
              << WEBVIEW_VERSION_MAJOR << "."
              << WEBVIEW_VERSION_MINOR << "."
              << WEBVIEW_VERSION_PATCH << "\n";

    if (check_only) {
        std::cout << "Link check passed.\n";
        return 0;
    }

    // Full smoke-test: open a window and close it immediately via JavaScript.
    try {
        webview::webview w(false, nullptr);
        w.set_title("Conan test_package");
        w.set_size(320, 240, WEBVIEW_HINT_NONE);
        w.set_html("<script>window.close();</script>");
        w.run();
    } catch (const webview::exception& e) {
        std::cerr << "webview error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
