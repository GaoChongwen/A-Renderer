#include "Vertex.hpp"

Vertex::Vertex(Vec3 pos , Color color, Vec3 normal , Vec2 tex):
pos(pos),
normal(normal),
tex(tex),
color(color){

}


Vertex Vertex::interpolate(const Vertex &vertex, double factor) const {
    Vec3 p = pos.interpolate(vertex.pos , factor);
    Vec3 nor = normal.interpolate(vertex.normal, factor);
    Vec2 t = tex.interpolate(vertex.tex , factor);
    Color c = color.interpolate(vertex.color, factor);
    return Vertex(p , c , nor , t);
}

void Vertex::transform(const Mat4 &mat4) {
//    pos = mat4.transform(pos);
}

VertexOut VertexOut::interpolateEarly(const VertexOut &target, double factor) const {
    VertexOut ret;
    ret.posWorld = posWorld.interpolate(target.posWorld, factor);
    ret.pos = pos.interpolate(target.pos , factor);
    ret.normal = normal.interpolate(target.normal, factor);
    ret.color = color.interpolate(target.color, factor);
    ret.posTrans = posTrans.interpolate(target.posTrans , factor);
    ret.tex = tex.interpolate(target.tex , factor);
    return ret;
}

double VertexOut::interpolateZ(const VertexOut &target, double factor) const {
    if (MathUtil::equal(oneDivZ , target.oneDivZ)) {
        return 1 / oneDivZ;
    }
    double _oneDivZ;
    _oneDivZ = MathUtil::interpolate(oneDivZ , target.oneDivZ , factor);
    return 1 / _oneDivZ;
}

VertexOut VertexOut::interpolate(const VertexOut &target, double factor) const {
    VertexOut ret;
    
    ret.pos = pos.interpolate(target.pos, factor);
    ret.oneDivZ = MathUtil::interpolate(oneDivZ , target.oneDivZ , factor);
    
    double z1 = getZ();
    double z = ret.getZ();
    double z2 = target.getZ();
    double cfactor;
    //透视校正
    if (z1 == z2) {
        cfactor = factor;
    } else {
        cfactor = (z - z1) / (z2 - z1);
    }
    ret.pos.z = MathUtil::interpolate(pos.z , target.pos.z, cfactor);
    ret.posWorld = posWorld.interpolate(target.posWorld, cfactor);
    ret.normal = normal.interpolate(target.normal , cfactor);
    ret.color = color.interpolate(target.color, cfactor);
    ret.posTrans = posTrans.interpolate(target.posTrans , cfactor);
    ret.tex = tex.interpolate(target.tex , cfactor);
    
    return ret;
}





