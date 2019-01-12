#ifndef Mesh_hpp
#define Mesh_hpp

#include "Vertex.hpp"
#include "Ref.hpp"
#include "Texture.hpp"
#include <vector>
#include "Shader.hpp"

class Sprite3D;

using namespace std;

class Mesh : public Ref {
    
public:
    
    static Mesh * create(const vector<Vertex> &vertice , const vector<int> &indice , const vector<const Texture *> &textures);
    
    void init(const vector<Vertex> &vertice , const vector<int> &indice , const vector<const Texture *> &textures);
    
    Mesh(){
        
    }
    
    Mesh(const vector<Vertex> &vertice , const vector<int> &indice , const vector<const Texture *> &textures) {
        init(vertice, indice, textures);
    }
    
    void draw(Shader * shader) const;
    
protected:
    
    vector<Vertex> _vertice;
    
    vector<const Texture *> _textures;
    
    vector<int> _indice;
    
    Sprite3D * _sprite;
};

#endif /* Mesh_hpp */
