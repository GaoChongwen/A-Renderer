#include "Box.hpp"
#include "Camera.hpp"
#include "Canvas.hpp"


Box::Box() :
_texture(nullptr){
    
}

Box::~Box() {
    
}

//控制box的旋转变�?
void Box::updateTransform(double dt) {
    // 旋转速度
    double velo = 45;
    // 角度变化
    _rotate.y += velo * dt;
    if (_rotate.y > 360) {
        _rotate.y -= 360;
    }
    // 矩阵旋转（角度转弧度�?
    Mat4 r = Mat4::rotateY(MathUtil::angle2radian(_rotate.y));
    // 模型空间到世界空间的变换矩阵
    Mat4 model = r * Mat4::scale(_scale) * Mat4::translate(_position);
    auto camera = Camera::getInstance();
    // 世界空间到相机空间的变换矩阵
    auto v = camera->getViewMat();
    // 投影矩阵
    auto p = camera->getProjectionMat();
    _shader->setMvp(model , v, p);
}

void Box::init() {
//    _texture = Texture::create("wall.jpg");
    _texture = Texture::create("wooden-crate.jpg");
    _texture_normal = Texture::create("wall_normal.jpg");
    double a[] = {
        //  X     Y     Z       U     V          Normal
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        
        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        
        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        
        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        
        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        
        // right
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f
    };
    
    unsigned size = sizeof(a) / sizeof(double);
    
    decltype(_vertice) temp;
    
    // 构造顶点集合，3i, 3i+1, 3i+2对应同一个三角形的三个顶�?
    for (int i = 0 ; i < size ; i = i + 8) {
        Vertex v = Vertex(Vec3(a[i] , a[i + 1] , a[i + 2]),
                          Color(1 , 1 , 1 , 0),
                          Vec3(a[i + 5] , a[i + 6] , a[i + 7]),
                          Vec2(a[i + 3] , a[i + 4]));
        _vertice.push_back(v);
    }
    
    Ambient ambient;
    ambient.color = Color(1,1,1,1);
    ambient.factor = 0.15;

    Light light;
    light.type = SpotLight;
    light.pos = Vec3(0 , 0 , 9);
    light.color = Color(1 , 1 , 1 , 1);
    light.factor = 1.7;
    light.range = 1.0;
    light.direction = Vec3(0,0,0);
    light.att = Vec3(0.1,0.1,0.1);

    Material material;
    material.diffuseFactor = 0.2;
    material.specularFactor = 0.7;
    material.shininess = 128;
    
    _shader = PhongShader::create();
    static_cast<PhongShader *>(_shader)->setMaterial(material);
    static_cast<PhongShader *>(_shader)->setLight(light);
    static_cast<PhongShader *>(_shader)->setAmbient(ambient);
}

void Box::draw(Ldouble dt) {
    begin(dt);
    Canvas::getInstance()->setTexture(_texture);
    Canvas::getInstance()->setTextureNormal(_texture_normal);
    Canvas::getInstance()->drawTrianglesFromVerts(_vertice);
    end();
}
