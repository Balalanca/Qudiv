
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <stdexcept>
#include <complex>
#include <cmath>

namespace qudiv::testing {

    inline int passed = 0;
    inline int failed = 0;

    inline void check(bool condition, const std::string& name) {
        if (condition) {
            std::cout << "[PASS] " << name << '\n';
            ++passed;
        }
        else {
            std::cout << "[FAIL] " << name << '\n';
            ++failed;
        }
    }

    template <typename ExceptionType>
    void check_throws(
        const std::function<void()>& operation,
        const std::string& name
    ) {
        try {
            operation();
            check(false, name + " (no exception)");
        }
        catch (const ExceptionType&) {
            check(true, name);
        }
        catch (const std::exception& e) {
            check(false, name + " (wrong exception: "
                + e.what() + ")");
        }
    }

    inline bool approximately_equal(double a, double b) {
        return std::abs(a - b) < 1e-12;
    }

    inline bool complex_equal(
        std::complex<double> a,
        std::complex<double> b
    ) {
        return approximately_equal(a.real(), b.real()) &&
            approximately_equal(a.imag(), b.imag());
    }
}
