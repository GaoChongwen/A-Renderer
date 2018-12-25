#ifndef Vec_h
#define Vec_h

#include <cmath>
#include <iostream>
#include "MathTool.h"
// ∂•µ„¿‡
class Vec3{
public:
	double x;
	double y;
	double z;

	Vec3(double x = 0.0f, double y = 0.0f, double z = 0.0f);

	double length() const;

	double normalize() const;

	double dot(const Vec3 &other) const;

	Vec3 cross(const Vec3 &other) const;

	Vec3 interpolate(const Vec3 &other, double factor) const;

	Vec3 operator + (const Vec3 &other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator - (const Vec3 &other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 operator * (const Vec3 &other) const {
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3 operator - () const {
		return Vec3(-x, -y, -z);
	}

	bool operator == (const Vec3 &other) const {
		return (MathTool::equal(x,other.x)&& MathTool::equal(y, other.y)&& MathTool::equal(z, other.z));
	}
};
#endif

