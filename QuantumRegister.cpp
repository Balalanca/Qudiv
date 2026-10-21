#include <utility>
#include "QuantumRegister.hpp"
#include <stdexcept>
#include <limits>


namespace qudiv {

	QuantumRegister::QuantumRegister(std::vector<std::size_t> dims) : dimensions (std::move(dims)), total_dimension_(dimensions_check()) {
		dimensions_check();
	}

	std::size_t QuantumRegister::size() const {
		return dimensions.size();
	}

	std::size_t QuantumRegister::dimension(std::size_t index) const {
		return dimensions.at(index);
	}

	std::size_t QuantumRegister::dimensions_check() const {
		if (dimensions.empty()) { throw std::invalid_argument("Quantum register cannot be empty."); }
		std::size_t tot = 1;
		constexpr std::size_t max = std::numeric_limits<std::size_t>::max();
		for (std::size_t dim : dimensions) {
			if (dim < 2) { throw std::invalid_argument("Qudit dimension must be at least 2."); }
			if (tot > max / dim) { throw std::overflow_error("Integer Overflow in Total Dimension."); }
			tot *= dim;
		}
		return tot;
	}

	std::size_t QuantumRegister::total_dimension() const {
		return total_dimension_;
	}
}