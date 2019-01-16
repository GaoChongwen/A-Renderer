#ifndef Vec_hpp
#define Vec_hpp

#include "Macro.h"
#include <cmath>
#include <iostream>
#include "MathUtil.hpp"

using namespace std;

class Vec3 {

public:
    union {
        double u;
        double x;
        double r;
    };
    
    union {
        double v;
        double y;
        double g;
    };
    
    union {
        double w;
        double z;
        double b;
    };
    
    double dot(const Vec3 &other) const;
    
    Vec3 reflect(const Vec3 &normal) const;
    
    Vec3 cross(const Vec3 &other) const;
    
    inline void print() const {
        cout << this << endl;
        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
        cout << "z = " << z << endl;
    }
    
    Vec3(double x = 0 , double y = 0 , double z = 0);
    
    inline Vec3 getNormalize() const {
        double length = getLength();
        Vec3 ret = Vec3(x / length , y / length , z / length);
        return ret;
    }
    
    inline double getLength() const {
        auto ret = sqrt((x * x + y * y + z * z));
        return ret;
    }
    
    Vec3 interpolate(const Vec3 &other , double factor) const {
        auto ret = *this + (other - *this) * factor;
        return ret;
    }
    
    Vec3 operator * (double factor) const {
        return Vec3(x * factor , y * factor , z * factor);
    }
    
    Vec3 operator + (const Vec3 &other) const {
        return Vec3(x + other.x , y + other.y , z + other.z);
    }
    
    Vec3 operator - (const Vec3 &other) const {
        return Vec3(x - other.x , y - other.y , z - other.z);
    }
    
    Vec3 operator - () const {
        return Vec3(-x , -y , -z);
    }
    
    bool operator == (const Vec3 &other) const {
        return MathUtil::equal(x, other.x)
            && MathUtil::equal(y, other.y)
            && MathUtil::equal(z, other.z);
    }
};

class Vec4 {
    
public:
    union {
        double r;
        double x;
    };
    
    union {
        double g;
        double y;
    };
    
    union {
        double b;
        double z;
    };
    
    union {
        double a;
        double w;
    };
    
    Vec4(const Vec3 &vec , double w = 1.0f){
        init(vec.x , vec.y , vec.z , w);
    }
    
    inline void print() const {
        cout << this << endl;
        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
        cout << "z = " << z << endl;
        cout << "w = " << w << endl;
    }
    
    Vec4(double x = 0.0f , double y = 0.0f , double z = 0.0f , double w = 1.0f);
    
    void init(double x = 0.0f , double y = 0.0f , double z = 0.0f , double w = 1.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    
    double dot(const Vec4 &other) const;
    
    Vec4 cross(const Vec4 &other) const;
    
    Vec4 interpolate(const Vec4 &other , double factor) const {
        auto ret = *this + (other - *this) * factor;
        return ret;
    }
    
    Vec3 getVec3() const {
        Vec3 ret(x , y , z);
        return ret;
    }
    
    Vec3 get3DNormal() const {
        Vec3 ret(x / w , y / w , z / w);
        return ret;
    }
    
    Vec4 operator + (const Vec4 &other) const {
        return Vec4(x + other.x , y + other.y , z + other.z , w + other.w);
    }
    
    Vec4 operator - (const Vec4 &other) const {
        auto ret = Vec4(x - other.x , y - other.y , z - other.z , w - other.w);
        return ret;
    }
    
    Vec4 operator * (const Vec4 &other) const {
        auto ret = Vec4(x * other.x , y * other.y , z * other.z , w * other.w);
        return ret;
    }
    
    Vec4 operator * (double factor) const {
        auto ret = Vec4(x * factor , y * factor , z * factor , w * factor);
        return ret;
    }
    
    Vec4 operator - () const {
        return Vec4(-x , -y , -z , -w);
    }
    
    bool operator == (const Vec4 &other) const {
        return MathUtil::equal(x, other.x)
        && MathUtil::equal(y, other.y)
        && MathUtil::equal(z , other.z)
        && MathUtil::equal(w , other.w);
    }
};


class Vec2 {
    
public:
    union {
        double u;
        double x;
    };
    
    union {
        double v;
        double y;
    };
    
    Vec2(double u = 0 , double v = 0):
    u(u),
    v(v) {
        
    }
    
    Vec2 interpolate(const Vec2 &other , double factor) const {
        auto ret = *this + (other - *this) * factor;
        return ret;
    }
    
    Vec2 operator * (double factor) const {
        return Vec2(u * factor , v * factor);
    }
    
    Vec2 operator + (const Vec2 &other) const {
        return Vec2(u + other.u , v + other.v);
    }
    
    Vec2 operator - (const Vec2 &other) const {
        return Vec2(u - other.u , v - other.v);
    }
    
    Vec2 operator - () const {
        return Vec2(-u , -v);
    }
    
    bool operator == (const Vec2 &other) const {
        return MathUtil::equal(u, other.u)
            && MathUtil::equal(v, other.v);
    }
};

#endif /* Vec_hpp */

















