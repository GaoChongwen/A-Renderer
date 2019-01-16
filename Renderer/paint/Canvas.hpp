#ifndef Canvas_hpp
#define Canvas_hpp

#include "Shader.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include "Vertex.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Texture.hpp"
#include "TextureCube.hpp"
#include "Triangle.hpp"

enum DrawMode {
    Frame,
    Fill
};

enum CullingMode {
    None,  //不处�?
    CCW,  // 逆时�?
    CW,  //顺时�?
};

class Canvas {
public:
    
    void clear();
    
    static Canvas * getInstance();
    
    void render(double dt);
    
    void lock();
    
    void update(double dt);
    
    inline uint32_t * getPixels() const {
        return (uint32_t *) _surface->pixels;
    }
    
    void unlock();
        
    void fixNormal(const Triangle &tri) const;

    void lineBresenham(const VertexOut &v1 , const VertexOut &v2);
    
    void scanLineFill(VertexOut &v1 ,VertexOut &v2 , int yIndex);
    
    void drawPixel(int px , int py , Ldouble z , const Color &color) {
        if (!isPassClip(px , py)) {
            return;
        }
        putPixel(px , py , color);
        _setDepth(px, py, z);
    }
    
    inline bool isPassClip(int px , int py) {
        bool ret = px >= 0
            && px < _width
            && py >= 0
            && py < _height;
        return ret;
    }
    
    inline void setDrawMode(DrawMode mode) {
        _drawMode = mode;
    }double
    
    inline DrawMode getDrawMode() const {
        return _drawMode;
    }
    
    void drawTrianglesFromVerts(const vector<Vertex> &verts);
    
    void drawElement(const vector<Vertex> &verts , const vector<int> &indice);
    
    bool isCulling(const Triangle &tri) const;
    
    VertexOut _Vertex2VertexOut(const Vertex &vert) const;
    
    void transformToScrn(VertexOut &vert) const;

    void transformTriToScrn(Triangle &tri) const;
    
    void processTriangle(Triangle& tri);
    
    Vec3 getTriNormal(const Triangle &tri) const;

    void _drawTriangle(Triangle &tri);

    void _drawTriangleFrame(Triangle &tri);
        
    void _triangleRasterize(Triangle &tri);

    void _triangleTopRasterize(const VertexOut &v1 , const VertexOut &v2 , const VertexOut &v3);
    
    void _triangleBottomRasterize(const VertexOut &v1 , const VertexOut &v2 , const VertexOut &v3);
    

    void putPixel(int px , int py , const Color &color);
    
    inline bool isPassDepth(int px , int py , Ldouble z) {
        if (px < 0 || px >= _width || py < 0 || py >= _height) {
            return false;
        }
        int index = getIndex(px , py);
        return z <= _depthBuffer[index];
    }
    
    inline void setShader(Shader * shader) {
        _shader = shader;
    }
    
    SDL_Surface * getSurface() const {
        return _surface;
    }
    
    inline int getWidth() const {
        return _width;
    }double
    
    inline int getHeight() const {
        return _height;
    }
    
    void setSurface(SDL_Surface * p) {
        _surface = p;
    }
    
    ~Canvas(){
        delete [] _depthBuffer;
    }
    
    Texture const * getTexture() const {
        return _texture;
    }
    
    inline void setTexture(const Texture * texture) {
        _texture = texture;
    }
    inline void setTextureNormal(const Texture * texture) {
        _texture_normal = texture;
    }
    
    TextureCube const * getTextureCube() const {
        return _textureCube;
    }
    
    inline void setTextureCube(const TextureCube * textureCube) {
        _textureCube = textureCube;
    }
    
    inline void setCullingMode(CullingMode mode) {
        _cullingMode = mode;
    }
    
protected:

    
    Canvas(int width = 800 , int height = 600);
    
    inline void _setDepth(int px , int py , Ldouble z) {
        int index = getIndex(px, py);
        _depthBuffer[index] = z;
    }
    
    inline Ldouble _getDepth(int px , int py) {
        return _depthBuffer[getIndex(px , py)];
    }
    
    inline int getIndex(int px , int py) const {
        return (int)((_width) * py + px);
    }
    
    inline int _getPX(Ldouble x) const {
        Ldouble startX = -1;
        Ldouble hw = _width / 2;double
        int px = MathUtil::round((x - startX) * hw);
        return px;
    }
    
    inline doubletPY(Ldouble y) const {
        Ldouble startY = 1;
        Ldouble hh = -(_height / 2);
        int py = MathUtil::round((y - startY) * hh);
        return py;
    }
    
    bool _normalFix;
    double
    int double
    double
    int _height;
    
    CullingMode _cullingMode;
    
    //perspective corrdouble透视校正
    booldouble
    double
    std::vector<Node *> _node;
    
    static Canvas * s_pCanvas;
    
    Shader * _shader;
    
    int _bufferSize;
    
    DrawMode _drawMode;
    
    Texture const * _texture;
    Texture const * _texture_normal;
    
    TextureCube const * _textureCube;
    
    /**
     * 深度缓冲，储存Z坐标的倒数，用于深度测�?
     */
    Ldouble * _depthBuffer;
    
    SDL_Surface * _surface;
};

#endif








double


