#pragma once
#include "Gate.hpp"

namespace qudiv {

	class XGate : public Gate {

	public:
		explicit XGate(std::size_t index);
		void apply(StateVector& state) const override;
	};

}