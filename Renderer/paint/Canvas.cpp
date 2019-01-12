#include "Canvas.hpp"
#include <algorithm>
#include <vector>
#include "Box.hpp"
#include <iostream>
#include "Bitmap.hpp"
#include "Sprite3D.hpp"
#include "Ground.hpp"
#include "SkyBox.hpp"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Canvas *Canvas::s_pCanvas = nullptr;

Canvas::Canvas(int width, int height) : _surface(nullptr),
                                        _width(width),
                                        _height(height),
                                        _drawMode(DrawMode::Fill),
                                        _cullingMode(CullingMode::None),
                                        _bufferSize(height * width),
                                        _texture(nullptr),
                                        _PC(true),
                                        _normalFix(true),
                                        _shader(nullptr),
                                        _textureCube(nullptr)
{
    _depthBuffer = new Ldouble[_bufferSize]();
    _shader = new Shader();

    //    auto sp = Sprite3D::create("nanosuit.obj");
    //    auto sp = Sprite3D::create("planet.obj");
    //    auto sp = Sprite3D::create("sample.obj");
    //    auto sp = Sprite3D::create("WusonOBJ.obj");
    //    sp->setPositionZ(-5);
    //    sp->setPositionY(-0);
    //    _node.push_back(sp);

    //    auto ground = Ground::create();
    //    _node.push_back(ground);
    //
    auto sky = SkyBox::create();
    _node.push_back(sky);

//    auto box = Box::create();
//    _node.push_back(box);
}

Canvas *Canvas::getInstance()
{
    if (s_pCanvas == nullptr)
    {
        s_pCanvas = new Canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    return s_pCanvas;
}

void Canvas::clear()
{
    memset(getPixels(), 0, sizeof(uint32_t) * _width * _height);
    std::fill(_depthBuffer, _depthBuffer + _bufferSize, MAXFLOAT);
}

void Canvas::update(double dt)
{

    lock();

    clear();

    render(dt);

    unlock();
}

void Canvas::render(double dt)
{
    for (int i = 0; i < _node.size(); ++i)
    {
        auto node = _node.at(i);
        node->draw(dt);
    }

}

void Canvas::lock()
{
    SDL_LockSurface(_surface);
}

void Canvas::unlock()
{
    SDL_UnlockSurface(_surface);
}

void Canvas::drawTrianglesFromVerts(const vector<Vertex> &verts)
{
    for (int i = 0; i < verts.size(); i = i + 3)
    {
        VertexOut vOut1 = _Vertex2VertexOut(verts.at(i));
        VertexOut vOut2 = _Vertex2VertexOut(verts.at(i + 1));
        VertexOut vOut3 = _Vertex2VertexOut(verts.at(i + 2));
        
        Triangle tri = Triangle(vOut1, vOut2, vOut3);
        processTriangle(tri);
    }
}

void Canvas::drawElement(const vector<Vertex> &verts, const vector<int> &indice)
{
    for (int i = 0; i < indice.size(); i = i + 3)
    {
        VertexOut vOut1 = _Vertex2VertexOut(verts.at(indice.at(i)));
        VertexOut vOut2 = _Vertex2VertexOut(verts.at(indice.at(i+1)));
        VertexOut vOut3 = _Vertex2VertexOut(verts.at(indice.at(i+2)));

        Triangle tri = Triangle(vOut1, vOut2, vOut3);
        processTriangle(tri);

    }
}

Vec3 Canvas::getTriNormal(const Triangle &tri) const
{
    const VertexOut &vOut1 = tri.v1;
    const VertexOut &vOut2 = tri.v2;
    const VertexOut &vOut3 = tri.v3;

    const Vec3 &pos1 = vOut1.posWorld;
    const Vec3 &pos2 = vOut2.posWorld;
    const Vec3 &pos3 = vOut3.posWorld;

    Vec3 v12 = pos2 - pos1;
    Vec3 v23 = pos3 - pos2;

    Vec3 normal = v12.cross(v23);

    return normal;
}

 void Canvas::fixNormal(const Triangle& tri) const
 {
    Vec3 normal = this->getTriNormal(tri);
    const VertexOut &vOut1 = tri.v1;
    const VertexOut &vOut2 = tri.v2;
    const VertexOut &vOut3 = tri.v3;

    vOut1.normal = normal;
    vOut2.normal = normal;
    vOut3.normal = normal;
 }


bool Canvas::isCulling(const Triangle &tri) const
{
    if (_cullingMode == None)
    {
        return false;
    }

    const VertexOut &vOut1 = tri.v1;
    const Vec3 &pos1 = vOut1.posWorld;

    Vec3 normal = this->getTriNormal(tri);

    Vec3 pos = Camera::getInstance()->getPosition();
    Vec3 dir = pos1 - pos;

    if (_cullingMode == CCW)
    {
        return dir.dot(normal) <= 0;
    }
    else
    {
        return dir.dot(normal) > 0;
    }
}

void Canvas::processTriangle(Triangle &tri)
{
    if (isCulling(tri)){
        return;
    }
    if (_normalFix)
    {
        fixNormal(tri);
    }
    
    Triangle triangle = tri;
    vector<Triangle> triangleList = {triangle};

    doClippingInCvv(triangleList);
    
     for (int i = 0; i < triangleList.size(); ++i)
     {
         Triangle &tri = triangleList.at(i);
         _drawTriangle(tri);
     }
}

void Canvas::transformTriToScrn(Triangle &tri) const
{
    transformToScrn(tri.v1);
    transformToScrn(tri.v2);
    transformToScrn(tri.v3);
}

void Canvas::_drawTriangleFrame(Triangle &tri){
    lineBresenham(tri.v1, tri.v2);
    lineBresenham(tri.v1, tri.v3);
    lineBresenham(tri.v3, tri.v2);
}


void Canvas::_drawTriangle(Triangle &tri)
{
    transformTriToScrn(tri);
    
    if (_drawMode == Fill)
    {
        _triangleRasterize(tri);
    }
    else if (_drawMode == Frame)
    {
        _drawTriangleFrame(tri);
    }
}

void Canvas::doClippingInCvv(vector<Triangle> &triList) const
{
    _doClppingInCvvAgainstNearPlane(triList);
    _doClppingInCvvAgainstFarPlane(triList);
    _doClppingInCvvAgainstLeftPlane(triList);
    _doClppingInCvvAgainstRightPlane(triList);
    _doClppingInCvvAgainstTopPlane(triList);
    _doClppingInCvvAgainstBottomPlane(triList);
}

void Canvas::_doClppingInCvvAgainstLeftPlane(vector<Triangle> &triList) const
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;

        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForLeftPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = - newW
                // Ldouble factor = (vertIn.pos.x + vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertIn.pos.w - vertOut.pos.w);
                Ldouble factor = _getFactorForLeftPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = - newW
                // Ldouble factor = (vertIn.pos.x + vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertIn.pos.w - vertOut.pos.w);
                Ldouble factor = _getFactorForLeftPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void Canvas::_doClppingInCvvAgainstRightPlane(vector<Triangle> &triList) const
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForRightPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = newW
                // Ldouble factor = (vertIn.pos.x - vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForRightPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = newW
                // Ldouble factor = (vertIn.pos.x - vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForRightPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void Canvas::_doClppingInCvvAgainstTopPlane(vector<Triangle> &triList) const
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;

        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForTopPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = newW
                // Ldouble factor = (vertIn.pos.y - vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForTopPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = newW
                // Ldouble factor = (vertIn.pos.y - vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForTopPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void Canvas::_doClppingInCvvAgainstBottomPlane(vector<Triangle> &triList) const
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForBottomPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = - newW
                // Ldouble factor = (vertIn.pos.y + vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertIn.pos.w - vertOut.pos.w);
                Ldouble factor = _getFactorForBottomPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = - newW
                // Ldouble factor = (vertIn.pos.y + vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertIn.pos.w - vertOut.pos.w);
                Ldouble factor = _getFactorForBottomPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void Canvas::_doClppingInCvvAgainstFarPlane(vector<Triangle> &triList) const
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForFarPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // az + (cz - az) * factor = newZ
                // aw + (cw - aw) * factor = newW
                // newZ = newW
                // Ldouble factor = (vertIn.pos.z - vertIn.pos.w) / (vertIn.pos.z - vertOut.pos.z + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForFarPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // az + (cz - az) * factor = newZ
                // aw + (cw - aw) * factor = newW
                // newZ = newW
                // Ldouble factor = (vertIn.pos.z - vertIn.pos.w) / (vertIn.pos.z - vertOut.pos.z + vertOut.pos.w - vertIn.pos.w);
                Ldouble factor = _getFactorForFarPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void Canvas::_doClppingInCvvAgainstNearPlane(vector<Triangle> &triList) const
{

    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForNearPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // Ldouble factor = (vertIn.pos.z - 0) / (vertIn.pos.z - vertOut.pos.z);
                Ldouble factor = _getFactorForNearPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                Ldouble factor = _getFactorForNearPlane(vertIn, vertOut);
                // VertexOut vertNew = (vertIn.pos.z - 0) / (vertIn.pos.z - vertOut.pos.z);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

vector<bool> Canvas::_getbOutListForNearPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.z < 0;
    bool bOut2 = p2.z < 0;
    bool bOut3 = p3.z < 0;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> Canvas::_getbOutListForFarPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;
    
    bool bOut1 = p1.z > p1.w;
    bool bOut2 = p2.z > p2.w;
    bool bOut3 = p3.z > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> Canvas::_getbOutListForRightPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.x > p1.w;
    bool bOut2 = p2.x > p2.w;
    bool bOut3 = p3.x > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;    
}

vector<bool> Canvas::_getbOutListForLeftPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.x < -p1.w;
    bool bOut2 = p2.x < -p2.w;
    bool bOut3 = p3.x < -p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> Canvas::_getbOutListForTopPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.y > p1.w;
    bool bOut2 = p2.y > p2.w;
    bool bOut3 = p3.y > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> Canvas::_getbOutListForBottomPlane(Triangle &tri) const
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.y < -p1.w;
    bool bOut2 = p2.y < -p2.w;
    bool bOut3 = p3.y < -p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

void Canvas::_triangleRasterize(Triangle &tri)
{

    VertexOut const *pVert1 = &(tri.v1);
    VertexOut const *pVert2 = &(tri.v2);
    VertexOut const *pVert3 = &(tri.v3);

    vector<VertexOut const *> vector = {pVert1, pVert2, pVert3};

    sort(vector.begin(), vector.end(), [](const VertexOut *p1, const VertexOut *p2) -> bool {
        return p1->pos.y >= p2->pos.y;
    });

    pVert1 = vector.at(0);
    pVert2 = vector.at(1);
    pVert3 = vector.at(2);

    if (MathUtil::equal(pVert1->pos.y, pVert2->pos.y))
    {
        _triangleBottomRasterize(*pVert1, *pVert2, *pVert3);
    }
    else if (MathUtil::equal(pVert2->pos.y, pVert3->pos.y))
    {
        _triangleTopRasterize(*pVert1, *pVert2, *pVert3);
    }
    else
    {
        Ldouble ty = pVert2->pos.y;
        Ldouble factor = (ty - pVert1->pos.y) / (pVert3->pos.y - pVert1->pos.y);
        VertexOut tVert = pVert1->interpolate(*pVert3, factor);
        _triangleTopRasterize(*pVert1, tVert, *pVert2);
        _triangleBottomRasterize(*pVert2, tVert, *pVert3);
    }
}

void Canvas::_triangleTopRasterize(const VertexOut &v1, const VertexOut &v2, const VertexOut &v3)
{
    const VertexOut *pVert1 = &v1;
    const VertexOut *pVert2 = &v2;
    const VertexOut *pVert3 = &v3;
    vector<const VertexOut *> vector = {pVert1, pVert2, pVert3};
    // 根据纵坐标排�?
    sort(vector.begin(), vector.end(), [](const VertexOut *p1, const VertexOut *p2) -> bool {
        return p1->pos.y >= p2->pos.y;
    });
    // 上面的顶�?
    pVert1 = vector.at(0);
    pVert2 = vector.at(1);
    pVert3 = vector.at(2);

    int startPY = pVert1->pos.y;
    int endPY = pVert3->pos.y;

    int sign = endPY > startPY ? 1 : -1;

    for (int py = startPY; py * sign <= sign * endPY; py = py + sign)
    {
        Ldouble ld = 1.0f;
        Ldouble factor = (py - startPY) * ld / (endPY - startPY);
        VertexOut vertStart = pVert1->interpolate(*pVert2, factor);
        VertexOut vertEnd = pVert1->interpolate(*pVert3, factor);
        scanLineFill(vertStart, vertEnd, py);
    }
}

void Canvas::_triangleBottomRasterize(const VertexOut &v1, const VertexOut &v2, const VertexOut &v3)
{
    const VertexOut *pVert1 = &v1;
    const VertexOut *pVert2 = &v2;
    const VertexOut *pVert3 = &v3;
    vector<const VertexOut *> vector = {pVert1, pVert2, pVert3};
    // 根据纵坐标排�?
    sort(vector.begin(), vector.end(), [](const VertexOut *p1, const VertexOut *p2) -> bool {
        return p1->pos.y >= p2->pos.y;
    });
    // 上面的顶�?
    pVert1 = vector.at(0);
    pVert2 = vector.at(1);
    pVert3 = vector.at(2);

    int startPY = pVert3->pos.y;
    int endPY = pVert1->pos.y;

    int sign = endPY > startPY ? 1 : -1;

    for (int py = startPY; py * sign < sign * endPY; py = py + sign)
    {
        Ldouble ld = 1.0f;
        Ldouble factor = (py - startPY) * ld / (endPY - startPY);
        VertexOut vertStart = pVert3->interpolate(*pVert2, factor);
        VertexOut vertEnd = pVert3->interpolate(*pVert1, factor);
        scanLineFill(vertStart, vertEnd, py);
    };
}

void Canvas::scanLineFill(const VertexOut &v1, const VertexOut &v2, int yIndex)
{

    const VertexOut *pVert1 = &v1;
    const VertexOut *pVert2 = &v2;

    pVert1 = v1.pos.x > v2.pos.x ? &v2 : &v1;
    pVert2 = v1.pos.x < v2.pos.x ? &v2 : &v1;

    int startX = pVert1->pos.x;
    int endX = pVert2->pos.x;

    if (startX == endX)
    {
        Ldouble z = pVert1->getZ();
        if (isPassDepth(startX, yIndex, z))
        {
            drawPixel(startX, yIndex, z, _shader->fs(*pVert1));
            return;
        }
    }
    for (int x = startX; x <= endX; ++x)
    {
        Ldouble factor = (x - startX) * 1.0f / (endX - startX);
        Ldouble z = pVert1->interpolateZ(*pVert2, factor);
        if (isPassDepth(x, yIndex, z))
        {
            VertexOut fragment = pVert1->interpolate(*pVert2, factor);
            drawPixel(x, yIndex, z, _shader->fs(fragment));
        }
    }
}

VertexOut Canvas::_Vertex2VertexOut(const Vertex &vert) const
{
    VertexOut vertOut = _shader->vs(vert);
    return vertOut;
}

void Canvas::transformToScrn(VertexOut &vert) const
{
    // 透视除法将cvv坐标转化成Ndc坐标
    vert.toNdc();
    // 转换到屏幕坐�?
    vert.pos.x = _getPX(vert.pos.x);
    vert.pos.y = _getPY(vert.pos.y);
}

void Canvas::putPixel(int px, int py, const Color &color)
{
    bool outX = px < 0 || px > _width;
    bool outY = py < 0 || py > _height;
    if (outX || outY)
    {
        return;
    }
    unsigned index = getIndex(px, py);
    auto pixels = getPixels();
    pixels[index] = color.uint32();
}

void Canvas::lineBresenham(const VertexOut &vert1, const VertexOut &vert2)
{
    const VertexOut *pVert1 = &vert1;
    const VertexOut *pVert2 = &vert2;

    int px1 = pVert1->pos.x;
    int py1 = pVert1->pos.y;

    int px2 = pVert2->pos.x;
    int py2 = pVert2->pos.y;

    if (px1 == px2 && py1 == py2)
    {
        Ldouble z = pVert1->getZ();
        if (isPassDepth(px1, py1, pVert1->getZ()))
        {
            drawPixel(px1, py1, z, _shader->fs(vert1));
        }
    }

    int dx = abs(px2 - px1);
    int dy = abs(py2 - py1);

    if (dx >= dy)
    {
        if (px1 > px2)
        {
            swap(pVert1, pVert2);
        }
        int px1 = pVert1->pos.x;
        int py1 = pVert1->pos.y;

        int px2 = pVert2->pos.x;
        int py2 = pVert2->pos.y;

        int sign = py2 >= py1 ? 1 : -1; //斜率[-1,1]
        int k = sign * dy * 2;
        int e = -dx * sign;

        for (int x = px1, y = py1; x <= px2; ++x)
        {
            Ldouble factor = static_cast<Ldouble>((x - px1) * 1.0 / (px2 - px1));
            Ldouble z = pVert1->interpolateZ(*pVert2, factor);
            if (isPassDepth(x, y, z))
            {
                VertexOut fragment = pVert1->interpolate(*pVert2, factor);
                drawPixel(x, y, z, _shader->fs(fragment));
            }
            e += k;
            if (sign * e > 0)
            {
                y += sign;
                e -= 2 * dx * sign;
            }
        }
    }
    else
    {
        if (py1 > py2)
        {
            swap(pVert1, pVert2);
        }
        int px1 = pVert1->pos.x;
        int py1 = pVert1->pos.y;

        int px2 = pVert2->pos.x;
        int py2 = pVert2->pos.y;

        int sign = px2 > px1 ? 1 : -1; //斜率[-1,1]
        int k = sign * dx * 2;
        int e = -dy * sign;

        for (int x = px1, y = py1; y <= py2; ++y)
        {
            Ldouble factor = static_cast<Ldouble>((y - py1) * 1.0 / (py2 - py1));
            Ldouble z = pVert1->interpolateZ(*pVert2, factor);
            if (isPassDepth(x, y, z))
            {
                VertexOut fragment = pVert1->interpolate(*pVert2, factor);
                drawPixel(x, y, z, _shader->fs(fragment));
            }
            e += k;
            if (sign * e > 0)
            {
                x += sign;
                e -= 2 * dy * sign;
            }
        }
    }
}
