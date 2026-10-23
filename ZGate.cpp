#include "ZGate.hpp"
#include <numbers>

namespace qudiv {

	ZGate::ZGate(std::size_t index) : Gate(index) {

	}

	void ZGate::apply(StateVector& state) const {
        // Check if the target index is valid
        const QuantumRegister& r = state.getRegister();

        if (index >= r.size()) {
            throw std::out_of_range("Invalid target qudit index");
        }

        // Calculate stride for the target qudit
        std::size_t stride = 1;

        for (std::size_t i = index + 1; i < r.size(); i++) {
            stride *= r.dimension(i);
        }

        std::size_t dim_i = r.dimension(index);
        
        std::complex<double> exponent(0.0, 2.0 * std::numbers::pi / dim_i);

        std::complex<double> omega = std::exp(exponent);

        for (std::size_t i = 0; i < r.total_dimension(); i++) {

            std::size_t j = (i / stride) % dim_i;
            state.amplitude(i) *= std::pow(omega, j);

        }

    }

}