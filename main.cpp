
#include <iostream>
#include "test_utils.hpp"

// Test functions defined in other .cpp files
namespace qudiv::testing {
    void run_register_tests();
    void run_statevector_tests();
    void testGates();
}

int main() {

    using namespace qudiv::testing;

    std::cout << "============================\n";
    std::cout << "         QUDIV TESTS\n";
    std::cout << "============================\n";

    // QuantumRegister tests
    run_register_tests();

    // StateVector tests
    run_statevector_tests();

    // XGate and ZGate tests
    testGates();

    std::cout << "\n============================\n";
    std::cout << "       FINAL RESULTS\n";
    std::cout << "============================\n";

    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "Total:  " << passed + failed << '\n';

    if (failed == 0) {
        std::cout << "\nALL TESTS PASSED!\n";
    }
    else {
        std::cout << "\nSOME TESTS FAILED!\n";
    }

    return failed == 0 ? 0 : 1;
}
