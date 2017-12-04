#pragma once

class Vec2 {
public:
	float x;
	float y;

	Vec2() {
		x = 0;
		y = 0;
	}

	explicit Vec2(float xy) {
		x = xy;
		y = xy;
	}

	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	static Vec2 random() {
		return Vec2((float)rand() / (RAND_MAX / 2) - 1, (float)rand() / (RAND_MAX / 2) - 1);
	}



	float length2() const {
		return x * x + y * y;
	}

	float length() const {
		return sqrt(length2());
	}

	void normalize() {
		float length = this->length();
		if (length != 0) {
			x /= length;
			y /= length;
		}
	}

	Vec2 normalized() const {
		float length = this->length();
		return length != 0 ? Vec2(x / length, y / length) : Vec2(0);
	}



	Vec2& operator+=(const Vec2& right) {
		x += right.x;
		y += right.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}

	Vec2& operator*=(const float right) {
		x *= right;
		y *= right;
		return *this;
	}

	Vec2& operator/=(const float right) {
		x /= right;
		y /= right;
		return *this;
	}

	Vec2 operator+(const Vec2& right) const {
		return Vec2(x + right.x, y + right.y);
	}

	Vec2 operator-(const Vec2& right) const {
		return Vec2(x - right.x, y - right.y);
	}

	Vec2 operator*(const float right) const {
		return Vec2(x * right, y * right);
	}

	Vec2 operator/(const float right) const {
		return right != 0 ? Vec2(x / right, y / right) : Vec2(0);
	}
};
