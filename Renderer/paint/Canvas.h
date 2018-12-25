//
//  Canvas.h
//  Renderer
//
//  Lipeng Liang
//  2018/12/24
//
#ifndef Canvas_h
#define Canvas_h

#include <stdio.h>
#include <SDL.h>
#include "math\Vec.h"


class Canvas {
protected:
	unsigned _width;

	unsigned _height;

	int _bufferSize;

	double * _depth;

	SDL_Surface * _surface;

public:

	Canvas();

	void clear();

	void render();

	void lock();

	void unlock();

	void putPixel(int x, int y, int z, uint32_t color);

	void drawLine();

	SDL_Surface * getSurface() const {
		return _surface;
	}

	void setSurface(SDL_Surface * p) {
		_surface = p;
	}
};

#endif // !Canvas_h
