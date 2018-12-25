//
//  Vertex.cpp
//  Renderer
//  Lipeng Liang
//  2018/12/24
//

#include "Vertex.h"

Vertex::Vertex(Vec3 pos, Color color, Vec3 normal, double u, double v) :pos(pos),normal(normal),u(u),v(v),color(color) {}

Vertex Vertex::interpolate(Vertex vertex, double factor) {
	Vec3 p = pos.interpolate(vertex.pos, factor);
	Vec3 nor = normal.interpolate(vertex.normal, factor);
	float tu = u + (vertex.u - u) * factor;
	float tv = v + (vertex.v - v) * factor;
	Color c = vertex.color.interpolate(vertex.color, factor);
	return Vertex(p, c, nor, tu, tv);
}
void Vertex::transform(const Mat4 &mat4) {
	pos = mat4.transform(pos);
}
