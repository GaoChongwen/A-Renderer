#include "Shader.hpp"
#include "Canvas.hpp"

VertexOut Shader::vs(const Vertex &vert) const {
    VertexOut vOut;
    auto temp = Vec4(vert.pos , 1.0f);
    vOut.posTrans = _mv.transform(temp);
    vOut.pos = _mvp.transform(Vec4(vert.pos , 1.0f));
    vOut.posWorld = _model.transform(Vec4(vert.pos , 1.0f)).get3DNormal();
    vOut.normal = _model.transform(Vec4(vert.normal , 0)).getVec3();
    vOut.color = vert.color;
    vOut.tex = vert.tex;
    
    return vOut;
}

void Shader::use(){
    Canvas::getInstance()->setShader(this);
}

void Shader::unUse() const {
    Canvas::getInstance()->setShader(nullptr);
}

Color Shader::fs(const VertexOut &frag) const {
    
    auto texture = Canvas::getInstance()->getTexture();
    
//    auto ret = texture->sample(vert.tex.u, vert.tex.v);
    
//    ret = vert.color;
    
    return frag.color;
}


void Shader::setTBN(Triangle &triangle){
    const VertexOut &vOut1 = triangle.v1;
    const VertexOut &vOut2 = triangle.v2;
    const VertexOut &vOut3 = triangle.v3;
    double p[4][4];
    for (int i=0; i<4; i++) {
        for (int j = 0; j<4; j++) {
            p[i][j]=0;
        }
    }
    
    double E0x = vOut2.pos.x - vOut1.pos.x;
    double E0y = vOut2.pos.y - vOut1.pos.y;
    double E0z = vOut2.pos.z - vOut1.pos.z;
    
    double E1x = vOut3.pos.x - vOut1.pos.x;
    double E1y = vOut3.pos.y - vOut1.pos.y;
    double E1z = vOut3.pos.z - vOut1.pos.z;
    
    double t1 = vOut2.tex.u - vOut1.tex.u;
    double b1 = vOut2.tex.v - vOut1.tex.v;
    
    double t2 = vOut3.tex.u - vOut1.tex.u;
    double b2 = vOut3.tex.v - vOut1.tex.v;
    
    double alpha = 1/(t1*b2 - b1*t2);
    
    double Tx = alpha*(b2*E0x - b1*E1x);
    double Ty = alpha*(b2*E0y - b1*E1y);
    double Tz = alpha*(b2*E0z - b1*E1z);
    
    double Bx = alpha*(-t2*E0x + t1*E1x);
    double By = alpha*(-t2*E0y + t1*E1y);
    double Bz = alpha*(-t2*E0z + t1*E1z);
    
    Vec4 line1(E0x, E0y, E0z, 0);
    Vec4 line2(E1x, E1y, E1z, 0);
    
    Vec4 N4 = line1.cross(line2);
    
    _TBN._11 = Tx;
    _TBN._12 = Ty;
    _TBN._13 = Tz;
    _TBN._21 = Bx;
    _TBN._22 = By;
    _TBN._23 = Bz;
    _TBN._31 = N4.x;
    _TBN._32 = N4.y;
    _TBN._33 = N4.z;
    _TBN._44 = 1;
}
