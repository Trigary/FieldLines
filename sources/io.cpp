#include "io.hpp"
#include <cstdio>
#include "configuration.hpp"
#include "line_data.hpp"
#include "charge_data.hpp"

namespace io {
	void print_info() {
		printf("Controls:\n");
		printf("Zoom: numpad +-\n");
		printf("Move: arrows\n");
		printf("Randomize coordinates: spacebar\n");
		printf("\n");

		printf("Darker means less...\n");
		printf("Darker circle -> less absolute eletric charge\n");
		printf("Darker line -> weaker electric field\n");
		printf("\n");
	}

	void initialize_charges() {
		printf("Number of lines: ");
		scanf_s("%u", &preferred_line_count);

		unsigned int count;
		printf("Number of charges: ");
		scanf_s("%u", &count);
		charge_data::initialize(count);

		float input;
		for (unsigned int i = 0; i < charge_data::count; i++) {
			printf("Electric charge of charge #%i: ", i + 1);
			scanf_s("%f", &input);
			charge_data::charges[i] = charge(vec2::random(), input);
		}

		line_data::calculate_vertices();
		charge_data::calculate_vertices();
	}
}
