#ifndef Vertex_hpp
#define Vertex_hpp

#include "Vec.hpp"
#include "Color.hpp"
#include "Mat4.hpp"

class Vertex {
   
public:
    Vec3 pos;
    Color color;
    Vec3 normal;
    Vec2 tex;
    
    Vertex(Vec3 pos = Vec3() , Color color = Color(), Vec3 normal = Vec3() , Vec2 tex = Vec2());
    
    Vertex interpolate(const Vertex &vertex , double factor) const;
    
    void transform(const Mat4 &mat4);
};

class VertexOut {
    
public:
    VertexOut interpolate(const VertexOut &target , double factor) const;
    
    VertexOut interpolateEarly(const VertexOut &target , double factor) const;
    
    double interpolateZ(const VertexOut &target , double factor) const;

    Color color;
    Vec3 color_normal;
    
    mutable Vec3 normal;
    
    //相机空间坐标
    Vec4 posTrans;
    
    Vec3 posWorld;
    
    Vec4 pos;
    
    Vec2 tex;
    
    inline double getZ() const {
        return 1 / oneDivZ;
    }
    
    inline void toNdc() {
        oneDivZ = 1 / pos.w;
        pos.x = pos.x / pos.w;
        pos.y = pos.y / pos.w;
        pos.z = pos.z / pos.w;
        pos.w = 1;
    }
    
protected:
    double oneDivZ = 0;
};

#endif /* Vertex_hpp */











