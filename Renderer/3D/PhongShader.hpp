#ifndef BoxShader_hpp
#define BoxShader_hpp

#include "Shader.hpp"
#include "Macro.h"

// struct Light {
//     Vec3 pos;
//     Color color;
//     Ldouble factor;
// };
struct Light {
    Color color;    // 颜色
    Ldouble factor; // 光照强度系数
    // 平行光
    Vec3 direction; // 方向

    // 点光源
    Vec3 pos;   // 光源位置
    Vec3 att;   // 衰减系数
    Ldouble range;  // 光照范围

    // flag
    LightType type; // 光照类型
};

struct Ambient {
    Color color;
    Ldouble factor;
};

struct Material {
    Ldouble diffuseFactor;
    Ldouble specularFactor;
    //
    Ldouble shininess;
};

class PhongShader : public Shader {
    
public:
    
    create_func(PhongShader)
    
    void setMaterial(const Material &material);
    
    void setLight(const Light &light);
    
    void setAmbient(const Ambient &ambient);
    
    virtual VertexOut vs(const Vertex &vertex) const;
    
    virtual Color fs(const VertexOut &frag) const;
    
    Color getDiffuse(const VertexOut &frag) const;
    
    Color getAmbient(const VertexOut &frag) const;
    
    Color getSpecular(const VertexOut &frag) const;
    
protected:
    Material _material;
    
    Ambient _ambient;
    
    Light _light;
};

#endif /* BoxShader_hpp */










