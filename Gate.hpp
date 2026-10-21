#pragma once

#include "StateVector.hpp"


namespace qudiv {


	class Gate {
		protected:
			std::size_t index;
		public:
			explicit Gate(std::size_t index);

			virtual ~Gate() = default;
			
			virtual void apply(StateVector& state) const = 0;
	};

}