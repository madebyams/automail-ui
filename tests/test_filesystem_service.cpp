#include "filesystem_service.hh"
#include "test_framework.hh"

namespace {
    TEST_CASE(filesystem_service_writes_and_reads_files) {
        temp_dir dir;
        filesystem_service service;
        const auto file_path = dir.path() / "sample.txt";

        service.store_file_content(file_path.string(), "hello backend tests");

        REQUIRE_EQ(service.get_file_content(file_path.string()), "hello backend tests");
    }

    TEST_CASE(filesystem_service_throws_for_missing_file) {
        temp_dir dir;
        filesystem_service service;
        const auto missing_path = dir.path() / "missing.txt";

        REQUIRE_THROWS_AS(service.get_file_content(missing_path.string()), std::runtime_error);
    }
}
