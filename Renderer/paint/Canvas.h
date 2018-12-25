//
//  Canvas.h
//  Renderer
//	画布类
//  Lipeng Liang
//  2018/12/24
//
#ifndef Canvas_h
#define Canvas_h

#include <SDL.h>
#include "math/Vec.h"
#include "paint/Color.h"
#include "Vertex.h"


class Canvas {
protected:
	int _width;

	int _height;

	unsigned _bufferSize;

	double * _depth;

	SDL_Surface * _surface;

	inline void _setDepth(int px, int py, double z) {
		unsigned index = getIndex(px, py);
		_depth[index] = z;
	}

	inline double _getDepth(int px, int py) {
		return _depth[getIndex(px, py)];
	}

	inline unsigned getIndex(int px, int py) const {
		return (unsigned)((_width)* py + px);
	}

	inline unsigned getPX(double x) const {
		double startX = -1;
		double hw = _width / 2;
		double px = (x - startX) * hw;
		return px;
	}

	inline unsigned getPY(double y) const {
		double startY = 1;
		double hh = -(_height / 2);
		double py = (y - startY) * hh;
		return py;
	}

public:

	Canvas(int width = 800, int height = 600);

	void clear();

	void render();

	void lock();

	void unlock();

	void update();


	

	void setSurface(SDL_Surface * p) {
		_surface = p;
	}

	SDL_Surface * getSurface() const {
		return _surface;
	}


	inline uint32_t * getPixels() const {
		return (uint32_t *)_surface->pixels;
	}

	// 画像素
	void putPixel(int px, int py, const Color &color);
	
	void drawPixel(int px, int py, double z, const Color &color) {
		if (!isPassDepth(px, py, z)) {
			return;
		}
		putPixel(px, py, color);
		_setDepth(px, py, z);
	}

	// 画点
	void drawPoint(double x, double y, double z, const Color &color);

	void drawPoint(const Vertex &vert);

	// 画线
	void drawLine(const Vertex &vert1, const Vertex &vert2);

	// 画三角形
	void drawTriangle(Vertex v1, Vertex v2, Vertex v3);

	inline bool isPassDepth(int px, int py, double z) {
		unsigned index = getIndex(px, py);
		return z <= _depth[index]
			&& z >= -1;
	}
	~Canvas() {
		delete[] _depth;
	}


};

#endif // !Canvas_h
