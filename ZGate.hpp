#pragma once
#include "Gate.hpp"

namespace qudiv {

	class ZGate : public Gate {

	public:
		explicit ZGate(std::size_t index);
		void apply(StateVector& state) const override;
	};

}