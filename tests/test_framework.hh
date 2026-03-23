#pragma once

#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct test_failure : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct test_case {
    std::string name;
    std::function<void()> fn;
};

std::vector<test_case>& registry();

struct registrar {
    registrar(std::string name, std::function<void()> fn) {
        registry().push_back({std::move(name), std::move(fn)});
    }
};

inline std::string to_string(bool value) {
    return value ? "true" : "false";
}

template <typename T>
std::string to_string(const T& value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

template <typename T, typename U>
void require_equal(const T& actual, const U& expected, const char* expression, const char* file, int line) {
    if (!(actual == expected)) {
        throw test_failure(
            std::string(file) + ":" + std::to_string(line) + " expected " + expression +
            " to equal [" + to_string(expected) + "] but was [" + to_string(actual) + "]"
        );
    }
}

inline void require_true(bool condition, const char* expression, const char* file, int line) {
    if (!condition) {
        throw test_failure(std::string(file) + ":" + std::to_string(line) + " check failed: " + expression);
    }
}

template <typename Exception, typename Fn>
void require_throws(Fn&& fn, const char* expression, const char* file, int line) {
    try {
        fn();
    } catch (const Exception&) {
        return;
    } catch (const std::exception& exception) {
        throw test_failure(
            std::string(file) + ":" + std::to_string(line) + " expected " + expression +
            " to throw requested exception type, but got: " + exception.what()
        );
    }

    throw test_failure(
        std::string(file) + ":" + std::to_string(line) + " expected " + expression +
        " to throw requested exception type"
    );
}

#define TEST_CASE(name) \
    void name(); \
    registrar name##_registrar{#name, name}; \
    void name()

#define REQUIRE(expr) require_true((expr), #expr, __FILE__, __LINE__)
#define REQUIRE_EQ(actual, expected) require_equal((actual), (expected), #actual, __FILE__, __LINE__)
#define REQUIRE_THROWS_AS(expr, exception_type) \
    require_throws<exception_type>([&]() { static_cast<void>(expr); }, #expr, __FILE__, __LINE__)

inline std::filesystem::path make_unique_temp_path() {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("automail-ui-" + std::to_string(stamp));
}

class temp_dir {
public:
    temp_dir() : path_(make_unique_temp_path()) {
        std::filesystem::create_directories(path_);
    }

    ~temp_dir() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    [[nodiscard]] const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

inline void set_env(const char* key, const char* value) {
#ifdef _WIN32
    _putenv_s(key, value);
#else
    setenv(key, value, 1);
#endif
}
