#pragma once

class vec2 {
public:
	float x;
	float y;

	vec2();
	explicit vec2(float xy);
	vec2(float x, float y);
	static vec2 random();

	float length2() const;
	float length() const;
	void normalize();
	vec2 normalized() const;

	vec2& operator+=(vec2& right);
	vec2& operator-=(vec2& right);
	vec2& operator*=(float right);
	vec2& operator/=(float right);

	vec2 operator+(const vec2& right) const;
	vec2 operator-(const vec2& right) const;
	vec2 operator*(float right) const;
	vec2 operator/(float right) const;
};
