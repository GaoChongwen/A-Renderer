#ifndef SkyBoxShader_hpp
#define SkyBoxShader_hpp

#include <stdio.h>

#include "Shader.hpp"

class SkyBoxShader : public Shader {
    
public:
    create_func(SkyBoxShader)
    
    void init();
    virtual VertexOut vs(const Vertex &vertex) const;
    virtual Color fs(const VertexOut &frag) const;
};

#endif /* SkyBoxShader_hpp */
