
#include "QuantumRegister.hpp"
#include "StateVector.hpp"
#include "test_utils.hpp"

#include <complex>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

namespace qudiv::testing {

    void run_statevector_tests() {

        // ============================================
        // GROUP 1: DEFAULT STATE
        // ============================================

        std::cout << "\n--- Default StateVector Tests ---\n";

        QuantumRegister qreg({ 2, 3 });
        StateVector default_state(qreg);

        check(default_state.size() == 6,
            "StateVector contains 6 amplitudes");

        check(
            complex_equal(
                default_state.amplitude(0),
                { 1.0, 0.0 }
            ),
            "Default state first amplitude is 1"
        );

        for (std::size_t i = 1; i < default_state.size(); ++i) {
            check(
                complex_equal(
                    default_state.amplitude(i),
                    { 0.0, 0.0 }
                ),
                "Default amplitude " + std::to_string(i)
                + " is zero"
            );
        }

        // ============================================
        // GROUP 2: CUSTOM COMPLEX STATE
        // ============================================

        std::cout << "\n--- Custom StateVector Tests ---\n";

        QuantumRegister qubit({ 2 });

        StateVector complex_state(
            qubit,
            {
                { 0.6, 0.0 },
                { 0.0, 0.8 }
            }
        );

        check(complex_state.size() == 2,
            "Custom qubit state has 2 amplitudes");

        check(
            complex_equal(
                complex_state.amplitude(0),
                { 0.6, 0.0 }
            ),
            "First complex amplitude correct"
        );

        check(
            complex_equal(
                complex_state.amplitude(1),
                { 0.0, 0.8 }
            ),
            "Second complex amplitude correct"
        );

        double total_probability = 0.0;

        for (std::size_t i = 0; i < complex_state.size(); ++i) {
            total_probability +=
                std::norm(complex_state.amplitude(i));
        }

        check(
            approximately_equal(total_probability, 1.0),
            "Complex state is normalized"
        );

        // ============================================
        // GROUP 3: ENTANGLED STATES
        // ============================================

        std::cout << "\n--- Entangled State Tests ---\n";

        QuantumRegister two_qubits({ 2, 2 });

        const double inv_sqrt2 = 1.0 / std::sqrt(2.0);

        StateVector bell(
            two_qubits,
            {
                { inv_sqrt2, 0.0 },
                { 0.0, 0.0 },
                { 0.0, 0.0 },
                { inv_sqrt2, 0.0 }
            }
        );

        check(bell.size() == 4,
            "Bell state has 4 amplitudes");

        check(
            complex_equal(
                bell.amplitude(0),
                { inv_sqrt2, 0.0 }
            ),
            "Bell state |00> amplitude correct"
        );

        check(
            complex_equal(
                bell.amplitude(3),
                { inv_sqrt2, 0.0 }
            ),
            "Bell state |11> amplitude correct"
        );

        // Mixed-dimensional entangled state
        // (|00> + |11>) / sqrt(2)

        QuantumRegister mixed({ 2, 3 });

        StateVector mixed_entangled(
            mixed,
            {
                { inv_sqrt2, 0.0 },
                { 0.0, 0.0 },
                { 0.0, 0.0 },
                { 0.0, 0.0 },
                { inv_sqrt2, 0.0 },
                { 0.0, 0.0 }
            }
        );

        check(mixed_entangled.size() == 6,
            "Qubit-qutrit entangled state has 6 amplitudes");

        check(
            complex_equal(
                mixed_entangled.amplitude(4),
                { inv_sqrt2, 0.0 }
            ),
            "Qubit-qutrit |11> amplitude correct"
        );

        // ============================================
        // GROUP 4: INVALID STATE VECTORS
        // ============================================

        std::cout << "\n--- Invalid StateVector Tests ---\n";

        // Unnormalized state

        check_throws<std::invalid_argument>(
            [&qubit]() {
                StateVector s(
                    qubit,
                    {
                        { 1.0, 0.0 },
                        { 1.0, 0.0 }
                    }
                );
            },
            "Unnormalized state rejected"
        );

        // Too few amplitudes

        check_throws<std::invalid_argument>(
            [&qubit]() {
                StateVector s(
                    qubit,
                    {
                        { 1.0, 0.0 }
                    }
                );
            },
            "Incorrect amplitude count rejected"
        );

        // Explicitly empty amplitude vector

        check_throws<std::invalid_argument>(
            [&qubit]() {
                StateVector s(qubit, {});
            },
            "Empty amplitude vector rejected"
        );

        // Zero vector

        check_throws<std::invalid_argument>(
            [&qubit]() {
                StateVector s(
                    qubit,
                    {
                        { 0.0, 0.0 },
                        { 0.0, 0.0 }
                    }
                );
            },
            "Zero vector rejected"
        );

        // NaN

        check_throws<std::invalid_argument>(
            [&qubit]() {
                double nan =
                    std::numeric_limits<double>::quiet_NaN();

                StateVector s(
                    qubit,
                    {
                        { nan, 0.0 },
                        { 0.0, 0.0 }
                    }
                );
            },
            "NaN amplitude rejected"
        );

        // Infinity

        check_throws<std::invalid_argument>(
            [&qubit]() {
                double inf =
                    std::numeric_limits<double>::infinity();

                StateVector s(
                    qubit,
                    {
                        { inf, 0.0 },
                        { 0.0, 0.0 }
                    }
                );
            },
            "Infinite amplitude rejected"
        );

        // Finite amplitude, overflowing squared magnitude

        check_throws<std::invalid_argument>(
            [&qubit]() {
                double huge = 1e308;

                StateVector s(
                    qubit,
                    {
                        { huge, 0.0 },
                        { 0.0, 0.0 }
                    }
                );
            },
            "Overflowing probability norm rejected"
        );

        // Invalid amplitude index

        check_throws<std::out_of_range>(
            [&default_state]() {
                default_state.amplitude(100);
            },
            "Invalid amplitude index rejected"
        );
    }

}
