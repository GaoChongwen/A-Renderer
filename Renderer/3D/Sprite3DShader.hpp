#ifndef Sprite3DShader_hpp
#define Sprite3DShader_hpp

#include <stdio.h>
#include "PhongShader.hpp"

class Sprite3DShader : public PhongShader {
    
public:
    
    create_func(Sprite3DShader)
    
    virtual VertexOut vs(const Vertex &vert) const;
    
    virtual Color fs(const VertexOut &frag) const;
    
};

#endif /* Sprite3DShader_hpp */
