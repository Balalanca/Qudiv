#include "StateVector.hpp"
#include <cmath>
#include <utility>


namespace qudiv {
	StateVector::StateVector(const QuantumRegister& reg, std::vector<std::complex<double>> amplitudes) : reg(reg), amplitudes(std::move(amplitudes)) {
		check_normalization();
	}

	StateVector::StateVector(const QuantumRegister& reg) : reg(reg) {
		assign_amplitudes();
		check_normalization();
	}

	void StateVector::assign_amplitudes(){
		amplitudes = std::vector<std::complex<double>>(size());
		amplitudes[0] = 1.0;
	}

	std::size_t StateVector::size() const {
		return reg.total_dimension();
	}

	std::complex<double> StateVector::amplitude(std::size_t index) const{
		return amplitudes.at(index);
	}

	std::complex<double>& StateVector::amplitude(std::size_t index) {
		return amplitudes.at(index);
	}

	const QuantumRegister& StateVector::getRegister() const{
		return reg;
	}

	void StateVector::check_normalization() const {
		// 3 cases: their squares add up to 1, same length as total_dimension, isfinite
		
		if(amplitudes.size() != reg.total_dimension()) {throw std::invalid_argument("Incorrect number of amplitudes.");}
		double tot = 0;
		const double tolerance = 1e-12;
		for (std::complex<double> amp : amplitudes) {
			if (!std::isfinite(amp.real())) { throw std::invalid_argument("Amplitude is not finite"); }
			if (!std::isfinite(amp.imag())) { throw std::invalid_argument("Amplitude is not finite"); }
			tot = tot + std::norm(amp);

		}
		if (std::abs(tot - 1) > tolerance) { throw std::invalid_argument("Unnormalized Amplitudes"); }
		if (!std::isfinite(tot)) { throw std::invalid_argument("Sum of squared amplitudes not finite"); }
	}

}