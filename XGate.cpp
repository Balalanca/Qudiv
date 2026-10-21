#include "XGate.hpp"


namespace qudiv {

	XGate::XGate(std::size_t index) : Gate(index){

	}

    void XGate::apply(StateVector& state) const {

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

        // Number of amplitudes in one block
        std::size_t blockSize = dim_i * stride;

        // Iterate over all blocks
        for (std::size_t block = 0; block < state.size(); block += blockSize) {

            // Iterate over the independent groups in each block
            for (std::size_t offset = 0; offset < stride; offset++) {

                std::size_t start = block + offset;

                // Cyclically shift amplitudes forward by one
                for (std::size_t j = 1; j < dim_i; j++) {
                    std::swap(
                        state.amplitude(start),
                        state.amplitude(start + j * stride)
                    );
                }
            }
        }
    }

}