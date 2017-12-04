#pragma once
#include "includes.h"
#include "lineData.hpp"
#include "chargeData.hpp"

namespace IO {
	inline void printInfo() {
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

	inline void initializeCharges() {
		printf("Number of lines: ");
		scanf_s("%u", &PREFERRED_LINE_COUNT);

		unsigned int count;
		printf("Number of charges: ");
		scanf_s("%u", &count);
		ChargeData::initialize(count);

		float input;
		for (unsigned int i = 0; i < ChargeData::count; i++) {
			printf("Electric charge of charge #%i: ", i + 1);
			scanf_s("%f", &input);
			ChargeData::charges[i] = Charge(Vec2::random(), input);
		}

		LineData::calculateVertices();
		ChargeData::calculateVertices();
	}
}
