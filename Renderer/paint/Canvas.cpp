//
//  Canvas.cpp
//  Renderer
//
//  Lipeng Liang
//  2018/12/24
//

#include "Canvas.h"
#include <vector>
#include <iostream>
#include <algorithm>


Canvas::Canvas(int width, int height) :
	_surface(nullptr),
	_width(width),
	_height(height),
	_bufferSize(height * width) {
	_depth = new double[_bufferSize]();
}

void Canvas::clear() {
	memset(getPixels(), 0, sizeof(uint32_t) * _width * _height);
	std::fill(_depth, _depth + _bufferSize, 1);
}

void Canvas::update() {

	lock();

	clear();

	render();

	unlock();

}

void Canvas::render() {
	for (double i = -1; i <= 1; i = i + 0.001) {
		//        putPixel(i , i, Color(1 , 1 ,1 ,1));
		drawPoint(i, i, i, Color(1, 0, 0, 0));
	}

	for (int i = 0; i < 600; ++i) {
		putPixel(i, i, Color(1, 1, 1, 1));
	}
}

void Canvas::lock() {
	SDL_LockSurface(_surface);
}

void Canvas::unlock() {
	SDL_UnlockSurface(_surface);
}

void Canvas::putPixel(int px, int py, const Color &color) {
	unsigned index = getIndex(px, py);
	auto pixels = getPixels();
	pixels[index] = color.uint32();
}

void Canvas::drawPoint(double x, double y, double z, const Color &color) {
	bool outX = x > 1 || x < -1;
	bool outY = y > 1 || y < -1;
	bool outZ = z > 1 || z < -1;

	if (outX || outY || outZ) {
		return;
	}

	double px = getPX(x);
	double py = getPY(y);

	unsigned index = getIndex(px, py);
	double depth = _depth[index];
	if (z > depth) {
		return;
	}

	putPixel(px, py, color);
}












