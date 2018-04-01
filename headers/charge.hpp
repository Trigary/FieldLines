#pragma once
#include "vector.hpp"

class charge {
public:
	vec2 position;
	float amount;

	charge();
	charge(vec2 position, float amount);
};
