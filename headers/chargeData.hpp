#pragma once
#include "includes.h"

namespace ChargeData {
	extern unsigned int count;
	extern Charge* charges;
	extern float* vertices;

	void initialize(unsigned int count);
	void clear();
	void calculateVertices();
}
