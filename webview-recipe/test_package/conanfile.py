import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class TestPackageConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        # webview opens a GUI window, so we only verify it compiled and linked.
        # Remove this guard if you want to run it in a headed environment.
        if can_run(self):
            bin_path = os.path.join(self.cpp.build.bindir, "test_package")
            # Pass --check so the binary exits immediately without opening a window.
            self.run(f'"{bin_path}" --check', env="conanrun")
