#ifndef Box_hpp
#define Box_hpp

#include "Vertex.hpp"
#include "PhongShader.hpp"
#include <vector>
#include "Node.hpp"

using namespace std;

class Texture;

class Box : public Node {
    
public:
    Box();
    ~Box();
    
    static Box * create() {
        Box *ret = new Box();
        ret->init();
        return ret;
    }
    
    void updateTransform(double dt);
    
    void init();
    
    void draw(double dt);
protected:
    
    Texture * _texture;
    Texture * _texture_normal;
    
    vector<Vertex> _vertice;
};

#endif /* Box_hpp */









