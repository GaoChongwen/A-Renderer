#ifndef Mat4_hpp
#define Mat4_hpp

#include "MathUtil.hpp"
#include "Vec.hpp"

class Mat4 {
    
public:
    union {
        double m[4][4];
        double a[16];
        struct
        {
            double _11; double _12; double _13; double _14;
            double _21; double _22; double _23; double _24;
            double _31; double _32; double _33; double _34;
            double _41; double _42; double _43; double _44;
        };
    };
    
    Mat4() {
        for (int i = 0 ;i < 16 ; ++ i) {
            a[i] = 0;
        }
    }
    
    Mat4(double p[4][4]) {
        for (int i = 0 ; i < 4; ++ i) {
            for (int j = 0 ; j < 4; ++ j) {
                m[i][j] = p[i][j];
            }
        }
    }
    
    Mat4(double * value) {
        for (int i = 0 ;i< 16 ; ++ i) {
            a[i] = value[i];
        }
    }
    
    inline void setZero() {
        for (int i = 0 ; i < 16; ++ i) {
            a[i] = 0;
        }
    }
    
    // 伴随矩阵除以行列�?
    Mat4 getInverseMat() const;
    
    Mat4 getTransposeMat() const;
    
    Mat4 getAdjointMat() const;
    
    double getAdjointElement(double a1, double a2, double a3,
                              double b1, double b2, double b3,
                              double c1, double c2, double c3)const;
    
    double getDet() const;
    
    inline void print() const {
        cout << this << endl;
        for (int i = 0 ; i < 4 ; ++ i) {
            for (int j = 0 ; j < 4; ++ j) {
                char * str = new char[50]();
                sprintf(str , "m[%d][%d] = %f  " , i , j , m[i][j]);
                cout << str ;
                delete [] str;
            }
            cout << endl;
        }
    }
    
    Vec4 transform(const Vec4 &vec) const;
    
    static Mat4 perspective(double fov , double aspect , double zNear , double zFar);

    static Mat4 scale(double scale);
    
    static Mat4 scale(double x , double y , double z);
    
    static Mat4 scale(const Vec3 &scale);
    
    static Mat4 translate(const Vec3 &vec);
    
    static Mat4 translate(double x , double y , double z);
    
    static Mat4 rotateX(double radian);
    
    static Mat4 rotateY(double radian);
    
    static Mat4 rotateZ(double radian);
    
    bool operator == (const Mat4 &other) const;
    
    Mat4 operator + (const Mat4 &other) const;
    
    Mat4 operator * (const Mat4 &other) const;
    
    Mat4 operator - (const Mat4 &other) const;
    
    Mat4 operator * (double num) const;
    
    Mat4 operator / (double num) const;
    
    static Mat4 identity();
    
};


#endif /* Mat4_hpp */
