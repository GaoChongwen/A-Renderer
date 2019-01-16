#ifndef Color_hpp
#define Color_hpp

#include <cstdlib>
#include <cstdint>
#include "MathUtil.hpp"

class Color {
    
public:
    union {
        struct {
            double r;
            double g;
            double b;
            double a;
        };
        double e[4];
    };
    
    Color(double r = 1.0f , double g = 1.0f , double b = 1.0f , double a = 1.0f):
    r(r),
    g(g),
    b(b),
    a(a){
    }
    
    Color(uint32_t rgba){
        r = ((rgba & 0xff000000) >> 24) / 255.0f;
        g = ((rgba & 0x00ff0000) >> 16) / 255.0f;
        b = ((rgba & 0x0000ff00) >> 8) / 255.0f;
        a = (rgba & 0x000000ff) / 255.0f;
    }
    
    static Color randomColor() {
        double r = (rand() % 255) / 255.0;
        double g = (rand() % 255) / 255.0;
        double b = (rand() % 255) / 255.0;
        return Color(r, g, b, 1);
    };
    
    uint32_t uint32() const {
        
        double fr = fmin(r, (double)1.0f);
        double fg = fmin(g, (double)1.0f);
        double fb = fmin(b, (double)1.0f);
        double fa = fmin(a, (double)1.0f);
        
        uint32_t R = (uint32_t)(fr * 255.0f);
        uint32_t G = (uint32_t)(fg * 255.0f);
        uint32_t B = (uint32_t)(fb * 255.0f);
        uint32_t A = (uint32_t)(fa * 255.0f);
        uint32_t value =B | (G << 8) | (R << 16) | (A << 24);
        return value;
    }
    
    Color operator + (const Color &color) const {
        auto ret = Color(r + color.r, g + color.g, b + color.b, a + color.a);
        return ret;
    };
    
    Color operator - (const Color &color) const {
        auto ret = Color(r - color.r, g - color.g, b - color.b, a - color.a);
        return ret;
    };
    
    Color operator * (double factor) const {
        auto ret = Color(r * factor, g * factor, b * factor, a * factor);
        return ret;
    };
    
    Color operator * (const Color &color) const {
        return Color(r * color.r , g * color.g , b * color.b , a * color.a);
    }
    
    Color interpolate(const Color &c, double factor) const {
        auto ret = *this + (c - *this) * factor;
        return ret;
    };
};

#endif /* Color_hpp */












