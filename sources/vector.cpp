#include "vector.hpp"
#include <cmath>
#include <cstdlib>

vec2::vec2() {
	x = 0;
	y = 0;
}

vec2::vec2(float xy) {
	x = xy;
	y = xy;
}

vec2::vec2(float x, float y) {
	this->x = x;
	this->y = y;
}

vec2 vec2::random() {
	return {
		((float)rand() / ((float)RAND_MAX / 2)) - 1,
		((float)rand() / ((float)RAND_MAX / 2)) - 1
	};
}



float vec2::length2() const {
	return x * x + y * y;
}

float vec2::length() const {
	return std::sqrt(length2());
}

void vec2::normalize() {
	float length = this->length();
	if (length != 0) {
		x /= length;
		y /= length;
	}
}

vec2 vec2::normalized() const {
	float length = this->length();
	return length != 0 ? vec2(x / length, y / length) : vec2(0);
}



vec2& vec2::operator+=(vec2& right) {
	x += right.x;
	y += right.y;
	return *this;
}

vec2& vec2::operator-=(vec2& right) {
	x -= right.x;
	y -= right.y;
	return *this;
}

vec2& vec2::operator*=(float right) {
	x *= right;
	y *= right;
	return *this;
}

vec2& vec2::operator/=(float right) {
	x /= right;
	y /= right;
	return *this;
}

vec2 vec2::operator+(const vec2& right) const {
	return {
		x + right.x,
		y + right.y
	};
}

vec2 vec2::operator-(const vec2& right) const {
	return {
		x - right.x,
		y - right.y
	};
}

vec2 vec2::operator*(float right) const {
	return {
		x * right,
		y * right
	};
}

vec2 vec2::operator/(float right) const {
	return right != 0 ? vec2(x / right, y / right) : vec2(0);
}
