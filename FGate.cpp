#include "FGate.hpp"
#include <numbers>
 

namespace qudiv {

	FGate::FGate(std::size_t index) : Gate(index) {

	}

	void FGate::apply(StateVector& state) const{
		const QuantumRegister& r = state.getRegister();
		if (index >= r.size()) {
			throw std::out_of_range("Not appropiate index");
		}

		const std::size_t d = r.dimension(index);
		const double normalization = 1.0 / std::sqrt(static_cast<double>(d));

		// stride
		std::size_t stride = 1;
		for (std::size_t i = index + 1; i < r.size(); i++) {
			stride *= r.dimension(i);
		}

		// blockSize
		std::size_t blockSize = d * stride;

		// number of blocks
		std::size_t Nblocks = r.total_dimension() / blockSize;

		// vector for original amplitudes
		std::vector<std::complex<double>> originals(d);
		std::vector<std::complex<double>> outputs(d);

		// loop over blocks
		for (std::size_t block = 0; block < Nblocks; block++) {
			
			// loop over group starting points within block
			for (std::size_t startp = 0; startp < stride; startp++) {
				
				// loop over amplitudes of group
				for (std::size_t amp = 0; amp < d; amp++) {
					std::size_t position = block * blockSize + startp + amp * stride;
					originals[amp] = state.amplitude(position);
				}

				// Apply the Quantum Fourier Transform
				// iterate over output amplitudes
				for (std::size_t amp = 0; amp < d; amp++) {
					// iterate over the summation formula
					std::complex<double> complex_am = {0.0, 0.0};
					for (std::size_t ind = 0; ind < d; ind++) {

						std::complex<double> exponent{
									0.0,
									2.0 * std::numbers::pi *
									static_cast<double>(ind) *
									static_cast<double>(amp) /
									static_cast<double>(d)
						};


						complex_am += originals[ind] * std::exp(exponent);
					}

					complex_am = complex_am * normalization;
					outputs[amp] = complex_am;

				}

				// replace the amplitudes
				for (std::size_t ind = 0; ind < d; ind++) {
					std::size_t position = block * blockSize + startp + ind * stride;

					// Write outputs[ind] to state at position.
					state.amplitude(position) = outputs[ind];
				}
			}

		}

	}

}