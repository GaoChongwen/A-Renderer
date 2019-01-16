#include "Mat4.hpp"


Vec4 Mat4::transform(const Vec4 &vec) const {
    double x = vec.x * _11 + vec.y * _21 + vec.z * _31 + vec.w * _41;
    double y = vec.x * _12 + vec.y * _22 + vec.z * _32 + vec.w * _42;
    double z = vec.x * _13 + vec.y * _23 + vec.z * _33 + vec.w * _43;
    double w = vec.x * _14 + vec.y * _24 + vec.z * _34 + vec.w * _44;
    return Vec4(x , y , z , w);
}

double Mat4::getDet() const {
    double result =
    _11 * _22 * _33 * _44   - _11 * _22 * _34 * _43   -
    _11 * _23 * _32 * _44   + _11 * _23 * _34 * _42   +
    
    _11 * _24 * _32 * _43   - _11 * _24 * _33 * _42   -
    _12 * _21 * _33 * _44   + _12 * _21 * _34 * _43   +
    
    _12 * _23 * _31 * _44   - _12 * _23 * _34 * _41   -
    _12 * _24 * _31 * _43   + _12 * _24 * _33 * _41   +
    
    _13 * _21 * _32 * _44   - _13 * _21 * _34 * _42   -
    _13 * _22 * _31 * _44   + _13 * _22 * _34 * _41   +
    
    _13 * _24 * _31 * _42   - _13 * _24 * _32 * _41   -
    _14 * _21 * _32 * _43   + _14 * _21 * _33 * _42   +
    
    _14 * _22 * _31 * _43   - _14 * _22 * _33 * _41   -
    _14 * _23 * _31 * _42   + _14 * _23 * _32 * _41 ; 
    
    return result;
}

double Mat4::getAdjointElement(double a1, double a2, double a3,
                               double b1, double b2, double b3,
                               double c1, double c2, double c3) const {
    return a1*(b2*c3 - c2*b3) - a2*(b1*c3 - c1*b3) + a3*(b1*c2 - c1*b2);
}

Mat4 Mat4::getAdjointMat() const {
    double a1 = getAdjointElement(_22, _23, _24, _32, _33, _34, _42, _43, _44);
    double a2 = getAdjointElement(_21, _23, _24, _31, _33, _34, _41, _43, _44);
    double a3 = getAdjointElement(_21, _22, _24, _31, _32, _34, _41, _42, _44);
    double a4 = getAdjointElement(_21, _22, _23, _31, _32, _33, _41, _42, _43);
    double b1 = getAdjointElement(_12, _13, _14, _32, _33, _34, _42, _43, _44);
    double b2 = getAdjointElement(_11, _13, _14, _31, _33, _34, _41, _43, _44);
    double b3 = getAdjointElement(_11, _12, _14, _31, _32, _34, _41, _42, _44);
    double b4 = getAdjointElement(_11, _12, _13, _31, _32, _33, _41, _42, _43);
    double c1 = getAdjointElement(_12, _13, _14, _22, _23, _24, _42, _43, _44);
    double c2 = getAdjointElement(_11, _13, _14, _21, _23, _24, _41, _43, _44);
    double c3 = getAdjointElement(_11, _12, _14, _21, _22, _24, _41, _42, _44);
    double c4 = getAdjointElement(_11, _12, _13, _21, _22, _23, _41, _42, _43);
    double d1 = getAdjointElement(_12, _13, _14, _22, _23, _24, _32, _33, _34);
    double d2 = getAdjointElement(_11, _13, _14, _21, _23, _24, _31, _33, _34);
    double d3 = getAdjointElement(_11, _12, _14, _21, _22, _24, _31, _32, _34);
    double d4 = getAdjointElement(_11, _12, _13, _21, _22, _23, _31, _32, _33);
    
    double arr[16] = {
        a1, -a2, a3, -a4,
        -b1, b2, -b3, b4,
        c1, -c2, c3, -c4,
        -d1, d2, -d3, d4
    };
    return Mat4(arr).getTransposeMat();
}

// 伴随矩阵除以行列�?
Mat4 Mat4::getInverseMat() const {
    Mat4 adj = getAdjointMat();
    double det = getDet();
    return adj / (det);
}

Mat4 Mat4::getTransposeMat() const {
    double a[16] = {
        _11, _21, _31, _41,
        _12, _22, _32, _42,
        _13, _23, _33, _43,
        _14, _24, _34, _44
    };
    return Mat4(a);
}

Mat4 Mat4::identity() {
    double ret[16] = {
        1 , 0 , 0 , 0,
        0 , 1 , 0 , 0,
        0 , 0 , 1 , 0,
        0 , 0 , 0 , 1
    };
    return Mat4(ret);
}

Mat4 Mat4::rotateX(double radian) {
    double s = sinf(radian);
    double c = cosf(radian);
    double values[16] = {
        1, 0,  0, 0,
        0, c,  s, 0,
        0, -s, c, 0,
        0, 0,  0, 1,
    };
    return Mat4(values);
}

Mat4 Mat4::rotateY(double radian) {
    double s = sinf(radian);
    double c = cosf(radian);
    double values[16] = {
        c, 0, -s, 0,
        0, 1, 0,  0,
        s, 0, c,  0,
        0, 0, 0,  1,
    };
    return Mat4(values);
}

Mat4 Mat4::rotateZ(double radian) {
    double s = sinf(radian);
    double c = cosf(radian);
    double values[16] = {
        c,  s, 0, 0,
        -s, c, 0, 0,
        0,  0, 1, 0,
        0,  0, 0, 1,
    };
    return Mat4(values);
}

Mat4 Mat4::perspective(double fovy, double aspect, double zNear, double zFar) {
    auto tanHalfFovy = tan(fovy / static_cast<double>(2));
//    double value[16] = {
//        static_cast<double>(1) / (aspect * tanHalfFovy) , 0 , 0 , 0,
//        0 , static_cast<double>(1) / (tanHalfFovy) , 0 , 0,
//        0 , 0 , - (zFar + zNear) / (zFar - zNear) , -1 ,
//        0 , 0 , - (static_cast<double>(2) * zFar * zNear) / (zFar - zNear) , 0
//    };
    
    double value[16] = {
        1.0f / (aspect * tanHalfFovy) , 0 , 0 , 0,
        0 , 1.0f / (tanHalfFovy) , 0 , 0,
        0 , 0 , - zFar / (zFar - zNear) , -1 ,
        0 , 0 , - (zFar * zNear) / (zFar - zNear) , 0
    };
    return Mat4(value);
}

Mat4 Mat4::scale(double scale) {
    return Mat4::scale(scale , scale , scale);
}

Mat4 Mat4::scale(const Vec3 &scale) {
    return Mat4::scale(scale.x , scale.y , scale.z);
}

Mat4 Mat4::scale(double x, double y, double z) {
    double a[16] = {
        x , 0 , 0 , 0,
        0 , y , 0 , 0,
        0 , 0 , z , 0,
        0 , 0 , 0 , 1
    };
    return Mat4(a);
}

Mat4 Mat4::translate(double x , double y , double z) {
    double a[16] = {
        1 , 0 , 0 , 0,
        0 , 1 , 0 , 0,
        0 , 0 , 1 , 0,
        x , y , z , 1
    };
    return Mat4(a);
}

Mat4 Mat4::translate(const Vec3 &vec) {
    return translate(vec.x , vec.y , vec.z);
}

Mat4 Mat4::operator * (const Mat4 &other) const {
    double ret[4][4];
    for (int i = 0 ; i < 4 ; ++ i) {
        for (int j = 0 ; j < 4 ; ++ j) {
            ret[i][j] = m[i][0] * other.m[0][j]
                    + m[i][1] * other.m[1][j]
                    + m[i][2] * other.m[2][j]
                    + m[i][3] * other.m[3][j];
        }
    }
    return Mat4(ret);
}

Mat4 Mat4::operator * (double num) const {
    double t[16] = {};
    for (int i = 0 ; i < 16 ; ++ i) {
        t[i] = a[i] * num;
    }
    return Mat4(t);
}

Mat4 Mat4::operator / (double num) const {
    double t[16] = {};
    for (int i = 0 ; i < 16 ; ++ i) {
        t[i] = a[i] / num;
    }
    return Mat4(t);
}

Mat4 Mat4::operator - (const Mat4 &other) const {
    double ret[16];
    for (int i = 0 ; i < 16; ++ i) {
        ret[i] = a[i] - other.a[i];
    }
    return Mat4(ret);
}

Mat4 Mat4::operator + (const Mat4 &other) const {
    double ret[16];
    for (int i = 0 ; i < 16; ++ i) {
        ret[i] = a[i] + other.a[i];
    }
    return Mat4(ret);
}

bool Mat4::operator == (const Mat4 &other) const {
    for (int i = 0 ; i < 16 ; ++ i) {
        if (! MathUtil::equal(a[i], other.a[i])) {
            return false;
        }
    }
    return true;
}

