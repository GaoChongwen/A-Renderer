//
//  Vertex.h
//  Renderer
//
//  Lipeng Liang
//  2018/12/24
//

#ifndef Vertex_h
#define Vertex_h

#include "Math\Vec.h"
#include "Color.h"

class Vertex {

public:
	Vec3 pos;
	Color color;
	Vec3 normal;
	double u;
	double v;

	Vertex(Vec3 pos, Vec3 normal, double u, double v, Color color) :pos(pos), normal(normal), u(u), v(v), color(color) {}


	Vertex interpolate(Vertex vertex, double factor);
};

#endif