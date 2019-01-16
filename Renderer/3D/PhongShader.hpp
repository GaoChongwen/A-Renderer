#ifndef BoxShader_hpp
#define BoxShader_hpp

#include "Shader.hpp"
#include "Macro.h"

// struct Light {
//     Vec3 pos;
//     Color color;
//     double factor;
// };
struct Light {
    Color color;    // ��ɫ
    double factor; // ����ǿ��ϵ��
    // ƽ�й�
    Vec3 direction; // ����

    // ���Դ
    Vec3 pos;   // ��Դλ��
    Vec3 att;   // ˥��ϵ��
    double range;  // ���շ�Χ

    // flag
    LightType type; // ��������
};

struct Ambient {
    Color color;
    double factor;
};

struct Material {
    double diffuseFactor;
    double specularFactor;
    //
    double shininess;
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
    
    Vec3 _Ray;
};

#endif /* BoxShader_hpp */










