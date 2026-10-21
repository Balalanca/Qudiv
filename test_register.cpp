
#include "QuantumRegister.hpp"
#include "test_utils.hpp"

#include <limits>
#include <stdexcept>

namespace qudiv::testing {

    void run_register_tests() {

        std::cout << "\n--- QuantumRegister Tests ---\n";

        QuantumRegister reg({ 2, 3, 4 });

        check(reg.size() == 3,
            "Register contains 3 qudits");

        check(reg.dimension(0) == 2,
            "First qudit dimension is 2");

        check(reg.dimension(1) == 3,
            "Second qudit dimension is 3");

        check(reg.dimension(2) == 4,
            "Third qudit dimension is 4");

        check(reg.total_dimension() == 24,
            "Total Hilbert dimension is 24");

        QuantumRegister reg2({ 2, 2, 2 });

        check(reg2.total_dimension() == 8,
            "Three qubits have dimension 8");

        QuantumRegister reg3({ 5 });

        check(reg3.total_dimension() == 5,
            "Single qudit has dimension 5");

        // Invalid registers

        check_throws<std::invalid_argument>(
            []() {
                QuantumRegister r({});
            },
            "Empty register rejected"
        );

        check_throws<std::invalid_argument>(
            []() {
                QuantumRegister r({ 2, 0, 4 });
            },
            "Zero-dimensional qudit rejected"
        );

        check_throws<std::invalid_argument>(
            []() {
                QuantumRegister r({ 2, 1, 4 });
            },
            "One-dimensional qudit rejected"
        );

        check_throws<std::out_of_range>(
            [&reg]() {
                reg.dimension(100);
            },
            "Invalid qudit index rejected"
        );

        check_throws<std::overflow_error>(
            []() {
                constexpr std::size_t max =
                    std::numeric_limits<std::size_t>::max();

                QuantumRegister r({ max, 2 });
            },
            "Register dimension overflow detected"
        );
    }

}
