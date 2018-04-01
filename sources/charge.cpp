#include "charge.hpp"

charge::charge() : position(0) {
	amount = 0;
}

charge::charge(vec2 position, float amount) : position(position) {
	this->amount = amount;
}
