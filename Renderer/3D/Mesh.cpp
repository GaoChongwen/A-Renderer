#include "Mesh.hpp"
#include "Canvas.hpp"

Mesh * Mesh::create(const vector<Vertex> &vertice, const vector<int> &indice, const vector<const Texture *> &textures) {
    Mesh * ret = new Mesh();
    ret->init(vertice , indice , textures);
    return ret;
}

void Mesh::init(const vector<Vertex> &vertice, const vector<int> &indice, const vector<const Texture *> &textures) {
    _vertice = vertice;
    _indice = indice;
    _textures = textures;
}

void Mesh::draw(Shader *shader) const {
    auto canvas = Canvas::getInstance();
    if (_textures.size() == 0) {
        canvas->setTexture(nullptr);
    } else {
        const Texture * texture = _textures.at(0);
        canvas->setTexture(texture);
    }
    canvas->drawElement(_vertice , _indice);
}
