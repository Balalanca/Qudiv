#pragma once

#include <vector>
#include <cstddef>

namespace qudiv {

	class QuantumRegister {

	private:

		std::vector<std::size_t> dimensions;
		std::size_t total_dimension_;
		std::size_t dimensions_check() const;
	

	public:

		QuantumRegister(std::vector<std::size_t> dims);

		std::size_t size() const;
		
		std::size_t dimension(std::size_t index) const;

		std::size_t total_dimension() const;

	};
}