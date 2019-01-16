#include "PhongShader.hpp"
#include "Canvas.hpp"
#include <algorithm>

void PhongShader::setLight(const Light &light) {
    _light = light;
}

void PhongShader::setMaterial(const Material &material) {
    _material = material;
}

void PhongShader::setAmbient(const Ambient &ambient) {
    _ambient = ambient;
}

VertexOut PhongShader::vs(const Vertex &vertex) const {
    return Shader::vs(vertex);
}

Color PhongShader::getAmbient(const VertexOut &frag) const {
    Color ambient = _ambient.color * _ambient.factor;
    return ambient;
}

Color PhongShader::getDiffuse(const VertexOut &frag) const {
    Vec3 fragPos = frag.posWorld;
    Vec3 normal = frag.normal.getNormalize();
    Vec3 color_normal = frag.color_normal;
    Vec3 ray = _light.direction; 
   
    if (_light.type == SpotLight){
        ray = (_light.pos - fragPos).getNormalize();
    }
    // Vec3 ray = (_light.pos - fragPos).getNormalize();
    auto TBN_inverse = _TBN.getInverseMat();
    Vec4 ray4;
    ray4.x = ray.x;
    ray4.y = ray.y;
    ray4.z = ray.z;
    ray4.w = 1;
    ray4 = this->_model.getInverseMat().transform(ray4);
    ray4 = TBN_inverse.transform(ray4);
    
    Vec3 new_ray;
    new_ray.x = ray4.x;
    new_ray.y = ray4.y;
    new_ray.z = ray4.z;
    
    double new_cosTheta = new_ray.dot(color_normal);
    
    double cosTheta = ray.dot(normal);
    
    double diff = max(cosTheta , (double)0.0f);
//    double diff = max(new_cosTheta , (double)0.0f);
//    diff = max(-new_cosTheta , diff);
    Color diffuse = _light.color* _light.factor * diff * _material.diffuseFactor;
    return diffuse;
}

Color PhongShader::getSpecular(const VertexOut &frag) const {
    Vec3 fragPos = frag.posWorld;
    Vec3 normal = frag.normal.getNormalize();
    Vec3 color_normal = frag.color_normal;
    
    Vec3 ray = _light.direction; 
    
    if (_light.type == SpotLight){
        ray = (_light.pos - fragPos).getNormalize();
    }
    
    
    // Vec3 ray = (_light.pos - fragPos).getNormalize();
    Vec3 cameraPos = Camera::getInstance()->getPosition();
    Vec3 viewDir = (cameraPos - fragPos).getNormalize();
    
    Vec3 center = (ray + viewDir).getNormalize();
    
    auto TBN_inverse = _TBN.getInverseMat();
    Vec4 center4;
    center4.x = center.x;
    centdouble center.y;
    centdouble center.z;
    center4.w = 1;
    center4 = this->_model.getInverseMat().transform(center4);
    center4 = TBN_inverse.transform(center4);
    
    Vec3 new_center;
    new_center.x = center4.x;
    new_center.y = center4.y;
    new_center.z = center4.z;
    
//    Vec3 reflectDir = (-ray).reflect(normal);
    
//    auto spec = pow(max(viewDir.dot(reflectDir), 0.0), _material.shininess);
    auto spec = pow(max(center.dot(normal), 0.0), _material.shininess);
//    auto temp = max(new_center.dot(color_normal), 0.0);
//    temp = max(-new_center.dot(color_normal), temp);
//    auto spec = pow(temp, _material.shininess);
//    auto spec = pow(max(new_center.dot(color_normal), 0.0), _material.shininess);
    Color specular = _light.color * _light.factor * spec * _material.specularFactor;
    return specular;
}

Color PhongShader::fs(const VertexOut &frag) const {
    auto texture = Canvas::getInstance()->getTexture();
    Color fragColor;
    if (texture == nullptr) {
        fragColor = frag.color;
    } else {
        fragColor = texture->sample(frag.tex.u, frag.tex.v);
    }
    
    Color ambient = getAmbient(frag);
    
    Color diffuse = getDiffuse(frag);
    
    Color specular = getSpecular(frag);

    if (_light.type == SpotLight){
        Vec3 fragPos = frag.posWorld;
        Ldouble length =(_light.pos - fragPos).getLength();
        Ldouble att = 1.f / _light.att.dot(Vec3(1.f, length, length*length));
//        diffuse = diffuse*att;
//        specular = specular*att;
    }

    Color phongColor = fragColor * (ambient + specular)+diffuse;

    return fragColor * (ambient + specular + diffuse);
}







