#pragma once

class Charge {
public:
	Vec2 position;
	float charge;

	Charge() : position(0) {
		charge = 0;
	}

	Charge(Vec2 position, float charge) : position(position) {
		this->charge = charge;
	}
};
