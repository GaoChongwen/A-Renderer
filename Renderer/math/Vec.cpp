#include "Vec.hpp"

Vec3::Vec3(double x , double y ,double z):
x(x),
y(y),
z(z){
    
}

Vec3 Vec3::reflect(const Vec3 &normal) const {
    Vec3 I = getNormalize();
    float tmp = 2.f * I.dot(normal);
    return I - (normal * tmp);
}

double Vec3::dot(const Vec3 &other) const {
    return x * other.x + y * other.y + z *other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
    double X = y * other.z - z * other.y;
    double Y = z * other.x - x * other.z;
    double Z = x * other.y - y * other.x;
    return Vec3(X , Y , Z);
}

Vec4::Vec4(double x , double y ,double z , double w):
x(x),
y(y),
z(z),
w(w){
    
}

double Vec4::dot(const Vec4 &other) const {
    return x * other.x + y * other.y + z *other.z;
}

Vec4 Vec4::cross(const Vec4 &other) const {
    double X = y * other.z - z * other.y;
    double Y = z * other.x - x * other.z;
    double Z = x * other.y - y * other.x;
    return Vec4(X , Y , Z , 0);
}











