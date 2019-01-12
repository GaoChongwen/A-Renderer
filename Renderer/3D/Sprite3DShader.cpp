#include "Sprite3DShader.hpp"

VertexOut Sprite3DShader::vs(const Vertex &vert) const {
    return PhongShader::vs(vert);
}

Color Sprite3DShader::fs(const VertexOut &frag) const {
    return PhongShader::fs(frag);
}
