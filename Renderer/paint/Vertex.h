//
//  Vertex.h
//  Renderer
//	œÚ¡ø¿‡
//  Lipeng Liang
//  2018/12/24
//

#ifndef Vertex_h
#define Vertex_h

#include "Math\Vec.h"
#include "Color.h"
#include "math/Mat4.h"

class Vertex {

public:
	Vec3 pos;
	Color color;
	Vec3 normal;
	double u;
	double v;

	Vertex(Vec3 pos, Color color = Color(), Vec3 normal = Vec3(), double u = 0, double v = 0);

	Vertex interpolate(Vertex vertex, double factor);

	void transform(const Mat4 &mat4);
};

#endif