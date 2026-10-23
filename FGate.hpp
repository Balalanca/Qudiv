#include "Gate.hpp"

namespace qudiv {

	class FGate : public Gate {

	public:
		explicit FGate(std::size_t index);
		void apply(StateVector& state) const override;
	};

}