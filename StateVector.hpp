#pragma once

#include <vector>
#include <cstddef>
#include <complex>
#include "QuantumRegister.hpp"

namespace qudiv {

	class StateVector {
	private:
		std::vector<std::complex<double>> amplitudes;
		QuantumRegister reg;
		void check_normalization() const;

	public:
		StateVector(const QuantumRegister& reg);
		
		StateVector(const QuantumRegister& reg, std::vector<std::complex<double>> amplitudes);

		std::size_t size() const;

		std::complex<double> amplitude(std::size_t index) const;

		void assign_amplitudes();
	};

}