#include "Mat4.h"

Mat4 Mat4::operator*(const Mat4 &other) const {

	double values[16];
	for (int index = 0; index < 16; index++) {
		int i = index / 4; // ��
		int j = index % 4; // ��
		values[index] = m[i * 4] * other.m[j]
			+ m[i * 4 + 1] * other.m[1 * 4 + j]
			+ m[i * 4 + 2] * other.m[2 * 4 + j]
			+ m[i * 4 + 3] * other.m[3 * 4 + j];
	}
	return Mat4(values);
}

Vec3 Mat4::transform(const Vec3 &vec) {
	double x = vec.x * m[0 * 4 + 0] + vec.y * m[1 * 4 + 0] + vec.z * m[2 * 4 + 0] + m[3 * 4 + 0];
	double y = vec.x * m[0 * 4 + 1] + vec.y * m[1 * 4 + 1] + vec.z * m[2 * 4 + 1] + m[3 * 4 + 1];
	double z = vec.x * m[0 * 4 + 2] + vec.y * m[1 * 4 + 2] + vec.z * m[2 * 4 + 2] + m[3 * 4 + 2];
	double w = vec.x * m[0 * 4 + 3] + vec.y * m[1 * 4 + 3] + vec.z * m[2 * 4 + 3] + m[3 * 4 + 3];
	return Vec3(x / w, y / w, z / w);
}

Mat4 Mat4::scale(const Vec3 &v) {
	return scale(v.x, v.y, v.z);
}

Mat4 Mat4::scale(double x, double y, double z) {
	double values[16] = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1,
	};
	return Mat4(values);
}

Mat4 Mat4::rotateX(double angle) {
	double s = sinf(angle);
	double c = cosf(angle);
	double values[16] = {
		1, 0,  0, 0,
		0, c,  s, 0,
		0, -s, c, 0,
		0, 0,  0, 1,
	};
	return Mat4(values);
}

Mat4 Mat4::rotateY(double angle) {
	double s = sinf(angle);
	double c = cosf(angle);
	double values[16] = {
		c, 0, -s, 0,
		0, 1, 0,  0,
		s, 0, c,  0,
		0, 0, 0,  1,
	};
	return Mat4(values);
}

Mat4 Mat4::rotateZ(double angle) {
	double s = sinf(angle);
	double c = cosf(angle);
	double values[16] = {
		c,  s, 0, 0,
		-s, c, 0, 0,
		0,  0, 1, 0,
		0,  0, 0, 1,
	};
	return Mat4(values);
}

Mat4 Mat4::rotate(const Vec3 &vec) {
	return rotate(vec.x, vec.y, vec.z);
}

Mat4 Mat4::rotate(double ax, double ay, double az) {
	Mat4 xm = Mat4::rotateX(ax);
	Mat4 ym = Mat4::rotateX(ay);
	Mat4 zm = Mat4::rotateX(az);
	return xm * ym * zm;
}

Mat4 Mat4::translate(const Vec3 &vec) {
	return translate(vec.x, vec.y, vec.z);
}

Mat4 Mat4::translate(double x, double y, double z) {
	double values[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1,
	};
	return Mat4(values);
}