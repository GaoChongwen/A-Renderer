//
//  Vertex.cpp
//  Renderer
//  Lipeng Liang
//  2018/12/24
//

#include "Vertex.h"

Vertex Vertex::interpolate(Vertex vertex, double factor) {
	Vec3 p = pos.interpolate(vertex.pos, factor);
	Vec3 nor = normal.interpolate(vertex.normal, factor);
	float tu = u + (vertex.u - u) * factor;
	float tv = v + (vertex.v - v) * factor;
	Color c = vertex.color.interpolate(vertex.color, factor);
	return Vertex(p, nor, tu, tv, c);
}
