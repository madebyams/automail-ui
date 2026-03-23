#include <iostream>
#include <vector>

#include "test_framework.hh"

std::vector<test_case>& registry() {
    static std::vector<test_case> tests;
    return tests;
}

int main() {
    int failures = 0;

    for (const auto& test : registry()) {
        try {
            test.fn();
            std::cout << "[PASS] " << test.name << '\n';
        } catch (const std::exception& exception) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << ": " << exception.what() << '\n';
        } catch (...) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << ": unknown exception\n";
        }
    }

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << registry().size() << " test(s) passed\n";
    return 0;
}
