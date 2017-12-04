#include "chargeData.hpp"

namespace ChargeData {
	unsigned int count = 0;
	Charge* charges = new Charge[0];
	float* vertices = new float[0];



	void initialize(unsigned int count) {
		clear();
		ChargeData::count = count;
		charges = new Charge[count];
		vertices = new float[count * 3];
	}

	void clear() {
		delete[] charges;
		delete[] vertices;
	}

	void calculateVertices() {
		float maxCharge = 0;
		float minCharge = 0;
		for (unsigned int i = 0; i < count; i++) {
			Charge charge = charges[i];
			unsigned int index = i * 3;
			vertices[index++] = charge.position.x;
			vertices[index] = charge.position.y;

			float cCharge = charge.charge;
			if (cCharge > 0) {
				if (cCharge > maxCharge) {
					maxCharge = cCharge;
				}
			} else {
				if (cCharge < minCharge) {
					minCharge = cCharge;
				}
			}
		}

		minCharge *= -1;
		for (unsigned int i = 0; i < count; i++) {
			float charge = charges[i].charge;
			unsigned int index = i * 3 + 2;

			if (charge > 0) {
				vertices[index] = charge / maxCharge;
			} else if (charge < 0) {
				vertices[index] = charge / minCharge;
			} else {
				vertices[index] = 0.0f;
			}
		}
	}
}
