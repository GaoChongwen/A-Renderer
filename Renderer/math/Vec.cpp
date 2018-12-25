//
// Vec.cpp
// Renderer
//
// Lipeng Liang on 2018/12/24
//

#include "Vec.h"

Vec3::Vec3(double x, double y, double z) :x(x), y(y), z(z) {}

double Vec3::length() const {
	return sqrtf(x*x + z * z + y * y);
}


double Vec3::dot(const Vec3 &other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
	float X = y * other.z - z * other.y;
	float Y = z * other.x - x * other.z;
	float Z = x * other.y - y * other.x;
	return Vec3(X, Y, Z);
}

// Ж«Ал
Vec3 Vec3::interpolate(const Vec3 &other, double factor) const {
	return *this + (other - *this) * factor;
}