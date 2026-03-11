import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import get, copy, rmdir
from conan.tools.build import check_min_cppstd
from conan.errors import ConanInvalidConfiguration


class WebviewConan(ConanFile):
    name = "webview"
    description = "A tiny cross-platform webview library for C/C++ to build modern cross-platform GUIs"
    license = "MIT"
    url = "https://github.com/webview/webview"
    homepage = "https://github.com/webview/webview"
    topics = ("webview", "gui", "html", "javascript", "webkit", "cross-platform")

    package_type = "header-library"
    settings = "os", "arch", "compiler", "build_type"

    # By default this is a header-only package exposing the C++ API.
    # Set build_library=True to also compile the shared/static C library.
    options = {
        "build_library": [True, False],
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "build_library": False,
        "shared": False,
        "fPIC": True,
    }

    # WebView2 SDK version fetched from NuGet on Windows (matches webview CMake default)
    _webview2_version = "1.0.1150.38"

    # -------------------------------------------------------------------------
    # Lifecycle
    # -------------------------------------------------------------------------

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if not self.options.build_library:
            # Header-only mode: shared/fPIC are irrelevant
            self.options.rm_safe("shared")
            self.options.rm_safe("fPIC")
            self.package_type = "header-library"
        else:
            self.package_type = "library"
            if self.options.shared:
                self.options.rm_safe("fPIC")

    def validate(self):
        if self.settings.os not in ("Linux", "Macos", "Windows"):
            raise ConanInvalidConfiguration(
                f"{self.ref} is not supported on {self.settings.os}"
            )
        min_std = "14" if self.settings.os == "Windows" else "11"
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, min_std)

    def layout(self):
        cmake_layout(self, src_folder="src")

    # -------------------------------------------------------------------------
    # Source
    # -------------------------------------------------------------------------

    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=True)

    # -------------------------------------------------------------------------
    # Build
    # -------------------------------------------------------------------------

    def generate(self):
        if not self.options.build_library:
            return
        tc = CMakeToolchain(self)
        tc.variables["WEBVIEW_BUILD_SHARED_LIBRARY"] = self.options.shared
        tc.variables["WEBVIEW_BUILD_STATIC_LIBRARY"] = not self.options.shared
        tc.variables["WEBVIEW_BUILD_TESTS"] = False
        tc.variables["WEBVIEW_BUILD_EXAMPLES"] = False
        tc.variables["WEBVIEW_BUILD_DOCS"] = False
        tc.variables["WEBVIEW_BUILD_AMALGAMATION"] = False
        tc.variables["WEBVIEW_INSTALL_TARGETS"] = True
        tc.generate()

    def build(self):
        if self.settings.os == "Windows":
            # WebView2 SDK headers are required on Windows even in header-only
            # mode. Download the NuGet package (which is a ZIP) and extract it.
            wv2 = self.conan_data["webview2"][self._webview2_version]
            get(self,
                url=wv2["url"],
                sha256=wv2["sha256"],
                destination=os.path.join(self.build_folder, "webview2_sdk"),
                strip_root=False)

        if not self.options.build_library:
            return

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    # -------------------------------------------------------------------------
    # Package
    # -------------------------------------------------------------------------

    def package(self):
        copy(self, "LICENSE*",
             src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))

        if self.settings.os == "Windows":
            # Bundle WebView2 SDK headers so consumers don't need NuGet separately
            webview2_include = os.path.join(
                self.build_folder, "webview2_sdk", "build", "native", "include"
            )
            copy(self, "*.h",
                 src=webview2_include,
                 dst=os.path.join(self.package_folder, "include"))

        if self.options.build_library:
            cmake = CMake(self)
            cmake.install()
            # Remove CMake config files — consumers use package_info() instead
            rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        else:
            # Header-only: copy the public include tree
            copy(self, "*.h",
                 src=os.path.join(self.source_folder, "core", "include"),
                 dst=os.path.join(self.package_folder, "include"))

    def package_id(self):
        if not self.info.options.build_library:
            # Headers vary by OS (WebView2 on Windows, WebKit on macOS) but not
            # by compiler, architecture, or build type.
            del self.info.settings.compiler
            del self.info.settings.build_type
            del self.info.settings.arch

    # -------------------------------------------------------------------------
    # Consumer info
    # -------------------------------------------------------------------------

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "webview")
        self.cpp_info.set_property("cmake_target_name", "webview::core")

        if self.options.build_library:
            # On MSVC the static lib is named webview_static to avoid a name
            # collision with the import library of the DLL build.
            if self.settings.os == "Windows" and not self.options.shared:
                self.cpp_info.libs = ["webview_static"]
            else:
                self.cpp_info.libs = ["webview"]
        else:
            self.cpp_info.bindirs = []
            self.cpp_info.libdirs = []

        # --- Platform system dependencies ---
        if self.settings.os == "Macos":
            self.cpp_info.frameworks = ["WebKit"]
            self.cpp_info.system_libs = ["dl"]

        elif self.settings.os == "Linux":
            self.cpp_info.system_libs = ["dl"]
            # GTK3 + WebKitGTK are required system packages resolved at build
            # time via pkg-config. Install them with:
            #   sudo apt install libgtk-3-dev libwebkit2gtk-4.1-dev
            # If your downstream consumers do not use CMake + pkg-config, add
            # the following line to expose the link flags explicitly:
            #   self.cpp_info.system_libs += ["gtk-3", "webkit2gtk-4.1"]

        elif self.settings.os == "Windows":
            self.cpp_info.system_libs = [
                "advapi32", "ole32", "shell32",
                "shlwapi", "user32", "version",
            ]
