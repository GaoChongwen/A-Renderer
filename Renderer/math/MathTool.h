#ifndef MathTool_h
#define MathTool_h

#include <stdio.h>
#include <cmath>
#include <algorithm>

#define Min 0.0000001
#define PI 3.141592653


class MathTool{
public:
	static inline bool equal(double x, double y) {
		return fabs(x - y) < Min;
	}

	static inline int round(double num) {
		return static_cast<int>(num + 0.5);
	}

	static inline double interpolate(double v1, double v2, double factor) {
		return v1 + (v2 - v1) * (factor);
	}
};

#endif // !MathTool_h
