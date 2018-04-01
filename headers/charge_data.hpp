#pragma once
#include "charge.hpp"

namespace charge_data {
	extern unsigned int count;
	extern charge* charges;
	extern float* vertices;

	void initialize(unsigned int count);
	void clear();
	void calculate_vertices();
}
