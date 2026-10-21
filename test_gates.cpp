
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <string>

#include "QuantumRegister.hpp"
#include "StateVector.hpp"
#include "XGate.hpp"
#include "ZGate.hpp"

namespace qudiv::testing {

    void testGates() {

        int passed = 0;
        int failed = 0;
        const double tolerance = 1e-9;

        auto check = [&](bool condition, const std::string& name) {
            if (condition) {
                std::cout << "[PASS] " << name << '\n';
                ++passed;
            }
            else {
                std::cout << "[FAIL] " << name << '\n';
                ++failed;
            }
            };

        auto close = [&](std::complex<double> actual,
            std::complex<double> expected) {
                return std::abs(actual - expected) < tolerance;
            };

        auto sameState = [&](const StateVector& a,
            const StateVector& b) {
                if (a.size() != b.size()) return false;

                for (std::size_t i = 0; i < a.size(); ++i) {
                    if (!close(a.amplitude(i), b.amplitude(i)))
                        return false;
                }
                return true;
            };

        auto normalized = [&](const StateVector& state) {
            double norm = 0.0;

            for (std::size_t i = 0; i < state.size(); ++i)
                norm += std::norm(state.amplitude(i));

            return std::abs(norm - 1.0) < tolerance;
            };

        std::cout << "\n===== XGATE TESTS =====\n";

        {
            QuantumRegister r({ 2, 3, 4 });
            const std::size_t expected[] = { 12, 4, 1 };

            for (std::size_t target = 0; target < 3; ++target) {
                StateVector state(r);
                XGate gate(target);
                gate.apply(state);

                bool correct = true;

                for (std::size_t i = 0; i < state.size(); ++i) {
                    std::complex<double> wanted =
                        i == expected[target]
                        ? std::complex<double>{1.0, 0.0}
                    : std::complex<double>{ 0.0, 0.0 };

                    correct &= close(state.amplitude(i), wanted);
                }

                check(correct, "X basis state target " + std::to_string(target));
                check(normalized(state), "X normalization target " + std::to_string(target));
            }

            const std::size_t dimensions[] = { 2, 3, 4 };

            for (std::size_t target = 0; target < 3; ++target) {
                StateVector state(r);
                StateVector original(r);
                XGate gate(target);

                for (std::size_t k = 0; k < dimensions[target]; ++k)
                    gate.apply(state);

                check(sameState(state, original),
                    "X^d identity target " + std::to_string(target));
            }

            StateVector state(r);
            XGate invalid(3);
            bool threw = false;

            try {
                invalid.apply(state);
            }
            catch (const std::out_of_range&) {
                threw = true;
            }
            catch (const std::invalid_argument&) {
                threw = true;
            }

            check(threw, "X invalid target");
        }

        {
            QuantumRegister r({ 2, 3, 4 });
            std::vector<std::complex<double>> amplitudes(24, { 0.0, 0.0 });

            double v = 1.0 / std::sqrt(3.0);
            amplitudes[0] = { v, 0.0 };
            amplitudes[6] = { v, 0.0 };
            amplitudes[23] = { v, 0.0 };

            const std::size_t expected[3][3] = {
                { 11, 12, 18 },
                { 4, 10, 15 },
                { 1, 7, 20 }
            };

            for (std::size_t target = 0; target < 3; ++target) {
                StateVector state(r, amplitudes);
                XGate gate(target);
                gate.apply(state);

                bool correct = true;

                for (std::size_t i = 0; i < state.size(); ++i) {
                    std::complex<double> wanted = { 0.0, 0.0 };

                    for (std::size_t k = 0; k < 3; ++k) {
                        if (i == expected[target][k])
                            wanted = { v, 0.0 };
                    }

                    correct &= close(state.amplitude(i), wanted);
                }

                check(correct, "X superposition target " + std::to_string(target));
                check(normalized(state), "X superposition normalization " + std::to_string(target));
            }
        }

        std::cout << "\n===== ZGATE TESTS =====\n";

        {
            QuantumRegister r({ 4 });
            std::vector<std::complex<double>> amplitudes(4, { 0.5, 0.0 });

            StateVector state(r, amplitudes);
            StateVector original(r, amplitudes);
            ZGate gate(0);

            gate.apply(state);

            check(close(state.amplitude(0), { 0.5, 0.0 }), "Z4 phase 0");
            check(close(state.amplitude(1), { 0.0, 0.5 }), "Z4 phase 1");
            check(close(state.amplitude(2), { -0.5, 0.0 }), "Z4 phase 2");
            check(close(state.amplitude(3), { 0.0, -0.5 }), "Z4 phase 3");
            check(normalized(state), "Z4 normalization");

            for (int k = 0; k < 3; ++k)
                gate.apply(state);

            check(sameState(state, original), "Z4^4 identity");
        }

        {
            QuantumRegister r({ 2, 3, 4 });
            std::vector<std::complex<double>> amplitudes(24, { 0.0, 0.0 });

            double v = 1.0 / std::sqrt(3.0);
            amplitudes[0] = { v, 0.0 };
            amplitudes[6] = { v, 0.0 };
            amplitudes[23] = { v, 0.0 };

            StateVector state(r, amplitudes);
            StateVector original(r, amplitudes);

            ZGate gate(1);
            gate.apply(state);

            const double pi = std::acos(-1.0);
            std::complex<double> omega = std::polar(1.0, 2.0 * pi / 3.0);

            bool correct = true;

            for (std::size_t i = 0; i < state.size(); ++i) {
                std::complex<double> wanted = { 0.0, 0.0 };

                if (i == 0) wanted = { v, 0.0 };
                if (i == 6) wanted = v * omega;
                if (i == 23) wanted = v * omega * omega;

                correct &= close(state.amplitude(i), wanted);
            }

            check(correct, "Z middle qutrit amplitudes");
            check(normalized(state), "Z mixed dimensions normalization");

            gate.apply(state);
            gate.apply(state);

            check(sameState(state, original), "Z3^3 identity");

            ZGate invalid(3);
            bool threw = false;

            try {
                invalid.apply(state);
            }
            catch (const std::out_of_range&) {
                threw = true;
            }
            catch (const std::invalid_argument&) {
                threw = true;
            }

            check(threw, "Z invalid target");
        }

        std::cout << "\n===== GATE TEST RESULTS =====\n";
        std::cout << "Passed: " << passed << '\n';
        std::cout << "Failed: " << failed << '\n';
        std::cout << "Total:  " << passed + failed << '\n';
    }

} // namespace qudiv
