
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
#include "FGate.hpp"

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



        std::cout << "\n===== FGATE TESTS =====\n";

        // Test 1: F2 must be equivalent to the Hadamard gate
        {
            QuantumRegister r({ 2 });
            StateVector state(r);
            FGate gate(0);

            gate.apply(state);

            const double v = 1.0 / std::sqrt(2.0);

            check(close(state.amplitude(0), { v, 0.0 }) &&
                close(state.amplitude(1), { v, 0.0 }),
                "F2 Hadamard on |0>");

            check(normalized(state), "F2 normalization");
        }

        // Test 2: F2 on |1> must produce (|0> - |1>)/sqrt(2)
        {
            QuantumRegister r({ 2 });
            std::vector<std::complex<double>> amplitudes = {
                { 0.0, 0.0 },{ 1.0, 0.0 }
            };

            StateVector state(r, amplitudes);
            FGate gate(0);
            gate.apply(state);

            const double v = 1.0 / std::sqrt(2.0);

            check(close(state.amplitude(0), { v, 0.0 }) &&
                close(state.amplitude(1), { -v, 0.0 }),
                "F2 Hadamard on |1>");
        }

        // Test 3: F3 on |1> must produce the correct complex phases
        {
            QuantumRegister r({ 3 });
            std::vector<std::complex<double>> amplitudes = {
                { 0.0, 0.0 },
                { 1.0, 0.0 },
                { 0.0, 0.0 }
            };

            StateVector state(r, amplitudes);
            FGate gate(0);
            gate.apply(state);

            const double v = 1.0 / std::sqrt(3.0);

            check(close(state.amplitude(0), { v, 0.0 }) &&
                close(state.amplitude(1), { -v / 2.0, 0.5 }) &&
                close(state.amplitude(2), { -v / 2.0, -0.5 }),
                "F3 complex phases on |1>");

            check(normalized(state), "F3 normalization");
        }

        // Test 4: F3 on |2> must produce the reversed phase pattern
        {
            QuantumRegister r({ 3 });
            std::vector<std::complex<double>> amplitudes = {
                { 0.0, 0.0 },
                { 0.0, 0.0 },
                { 1.0, 0.0 }
            };

            StateVector state(r, amplitudes);
            FGate gate(0);
            gate.apply(state);

            const double v = 1.0 / std::sqrt(3.0);

            check(close(state.amplitude(0), { v, 0.0 }) &&
                close(state.amplitude(1), { -v / 2.0, -0.5 }) &&
                close(state.amplitude(2), { -v / 2.0, 0.5 }),
                "F3 complex phases on |2>");
        }

        // Test 5: F_d^4 = I for dimensions 2 through 10
        {
            for (std::size_t d = 2; d <= 10; ++d) {
                QuantumRegister r({ d });

                std::vector<std::complex<double>> amplitudes(
                    d, { 0.0, 0.0 });

                const double v = 1.0 / std::sqrt(2.0);
                amplitudes[0] = { v, 0.0 };
                amplitudes[1] = { 0.0, v };

                StateVector state(r, amplitudes);
                StateVector original(r, amplitudes);

                FGate gate(0);

                for (int n = 0; n < 4; ++n)
                    gate.apply(state);

                check(sameState(state, original),
                    "F^4 identity dimension " + std::to_string(d));

                check(normalized(state),
                    "F^4 normalization dimension " + std::to_string(d));
            }
        }

        // Test 6: Fourier on the middle qudit of (2,3,2)
        {
            QuantumRegister r({ 2, 3, 2 });
            std::vector<std::complex<double>> amplitudes(
                12, { 0.0, 0.0 });

            // Prepare |010>, flattened index 2
            amplitudes[2] = { 1.0, 0.0 };

            StateVector state(r, amplitudes);
            FGate gate(1);
            gate.apply(state);

            std::vector<std::complex<double>> expected(
                12, { 0.0, 0.0 });

            const double v = 1.0 / std::sqrt(3.0);
            expected[0] = { v, 0.0 };
            expected[2] = { -v / 2.0, 0.5 };
            expected[4] = { -v / 2.0, -0.5 };

            bool correct = true;

            for (std::size_t i = 0; i < state.size(); ++i)
                correct &= close(state.amplitude(i), expected[i]);

            check(correct, "F3 middle qutrit indexing");
            check(normalized(state), "F3 middle qutrit normalization");
        }

        // Test 7: Fourier on the first qudit of (2,3,2)
        {
            QuantumRegister r({ 2, 3, 2 });
            std::vector<std::complex<double>> amplitudes(
                12, { 0.0, 0.0 });

            // Prepare |100>, flattened index 6
            amplitudes[6] = { 1.0, 0.0 };

            StateVector state(r, amplitudes);
            FGate gate(0);
            gate.apply(state);

            std::vector<std::complex<double>> expected(
                12, { 0.0, 0.0 });

            const double v = 1.0 / std::sqrt(2.0);
            expected[0] = { v, 0.0 };
            expected[6] = { -v, 0.0 };

            bool correct = true;

            for (std::size_t i = 0; i < state.size(); ++i)
                correct &= close(state.amplitude(i), expected[i]);

            check(correct, "F2 first qudit indexing");
        }

        // Test 8: Fourier on the last qudit of (2,3,2)
        {
            QuantumRegister r({ 2, 3, 2 });
            std::vector<std::complex<double>> amplitudes(
                12, { 0.0, 0.0 });

            // Prepare |101>, flattened index 7
            amplitudes[7] = { 1.0, 0.0 };

            StateVector state(r, amplitudes);
            FGate gate(2);
            gate.apply(state);

            std::vector<std::complex<double>> expected(
                12, { 0.0, 0.0 });

            const double v = 1.0 / std::sqrt(2.0);
            expected[6] = { v, 0.0 };
            expected[7] = { -v, 0.0 };

            bool correct = true;

            for (std::size_t i = 0; i < state.size(); ++i)
                correct &= close(state.amplitude(i), expected[i]);

            check(correct, "F2 last qudit indexing");
        }

        // Test 9: Independent groups with a nontrivial superposition
        {
            QuantumRegister r({ 2, 3, 2 });
            std::vector<std::complex<double>> amplitudes(
                12, { 0.0, 0.0 });

            // Superposition across two independent Fourier groups
            const double v = 1.0 / std::sqrt(2.0);

            amplitudes[0] = { v, 0.0 };
            amplitudes[7] = { v, 0.0 };

            StateVector state(r, amplitudes);
            FGate gate(1);
            gate.apply(state);

            std::vector<std::complex<double>> expected(
                12, { 0.0, 0.0 });

            const double a = 1.0 / std::sqrt(6.0);

            // Group (0,2,4): Fourier applied to local |0>
            expected[0] = { a, 0.0 };
            expected[2] = { a, 0.0 };
            expected[4] = { a, 0.0 };

            // Group (7,9,11): Fourier applied to local |0>
            expected[7] = { a, 0.0 };
            expected[9] = { a, 0.0 };
            expected[11] = { a, 0.0 };

            bool correct = true;

            for (std::size_t i = 0; i < state.size(); ++i)
                correct &= close(state.amplitude(i), expected[i]);

            check(correct, "F3 independent amplitude groups");
            check(normalized(state), "F3 superposition normalization");
        }

        // Test 10: F^4 = I on every target of a mixed-dimension register
        {
            QuantumRegister r({ 2, 3, 4 });

            std::vector<std::complex<double>> amplitudes(
                24, { 0.0, 0.0 });

            const double v = 1.0 / std::sqrt(3.0);

            amplitudes[0] = { v, 0.0 };
            amplitudes[6] = { 0.0, v };
            amplitudes[23] = { -v, 0.0 };

            for (std::size_t target = 0; target < 3; ++target) {
                StateVector state(r, amplitudes);
                StateVector original(r, amplitudes);

                FGate gate(target);

                for (int n = 0; n < 4; ++n)
                    gate.apply(state);

                check(sameState(state, original),
                    "F^4 mixed dimensions target " +
                    std::to_string(target));

                check(normalized(state),
                    "F mixed dimensions normalization target " +
                    std::to_string(target));
            }
        }

        // Test 11: Arbitrary complex superposition must preserve norm
        {
            QuantumRegister r({ 5 });

            std::vector<std::complex<double>> amplitudes = {
                { 1.0,  2.0 },
                { -2.0, 1.0 },
                { 0.0, -3.0 },
                { 4.0,  0.0 },
                { -1.0, -1.0 }
            };

            double normSquared = 0.0;

            for (const auto& amplitude : amplitudes)
                normSquared += std::norm(amplitude);

            const double scale = 1.0 / std::sqrt(normSquared);

            for (auto& amplitude : amplitudes)
                amplitude *= scale;

            StateVector state(r, amplitudes);
            FGate gate(0);
            gate.apply(state);

            check(normalized(state),
                "F5 arbitrary complex state normalization");
        }

        // Test 12: Invalid target must throw an exception
        {
            QuantumRegister r({ 2, 3, 4 });
            StateVector state(r);

            FGate invalid(3);
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

            check(threw, "F invalid target");
        }




        std::cout << "\n===== GATE TEST RESULTS =====\n";
        std::cout << "Passed: " << passed << '\n';
        std::cout << "Failed: " << failed << '\n';
        std::cout << "Total:  " << passed + failed << '\n';
    }

} // namespace qudiv
