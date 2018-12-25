//
//  Canvas.h
//  Renderer
//	ª≠≤º¿‡
//  Lipeng Liang
//  2018/12/24
//
#ifndef Canvas_h
#define Canvas_h

#include <SDL.h>
#include "math/Vec.h"
#include "paint/Color.h"


class Canvas {
protected:
	int _width;

	int _height;

	unsigned _bufferSize;

	double * _depth;

	SDL_Surface * _surface;

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

	inline unsigned getIndex(int px, int py) const {
		return (unsigned)(_width * py + px);
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

	inline uint32_t * getPixels() const {
		return (uint32_t *)_surface->pixels;
	}

	//void drawLine();

	void putPixel(int px, int py, const Color &color);

	void drawPoint(double x, double y, double z, const Color &color);

	~Canvas() {
		delete[] _depth;
	}
};

#endif // !Canvas_h
