#include "charge_data.hpp"

namespace charge_data {
	unsigned int count = 0;
	charge* charges = new charge[0];
	float* vertices = new float[0];



	void initialize(unsigned int count) {
		clear();
		charge_data::count = count;
		charges = new charge[count];
		vertices = new float[count * 3];
	}

	void clear() {
		delete[] charges;
		delete[] vertices;
	}

	void calculate_vertices() {
		float max_charge = 0;
		float min_charge = 0;
		for (unsigned int i = 0; i < count; i++) {
			charge charge = charges[i];
			unsigned int index = i * 3;
			vertices[index++] = charge.position.x;
			vertices[index] = charge.position.y;

			float amount = charge.amount;
			if (amount > max_charge) {
				max_charge = amount;
			} else if (amount < min_charge) {
				min_charge = amount;
			}
		}

		min_charge *= -1;
		for (unsigned int i = 0; i < count; i++) {
			float charge = charges[i].amount;
			unsigned int index = i * 3 + 2;

			if (charge > 0) {
				vertices[index] = charge / max_charge;
			} else if (charge < 0) {
				vertices[index] = charge / min_charge;
			} else {
				vertices[index] = 0.0f;
			}
		}
	}
}
