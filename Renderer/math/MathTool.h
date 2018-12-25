#ifndef MathTool_h
#define MathTool_h

#include <stdio.h>
#include <cmath>
#include <algorithm>

#define Min 0.0000001
#define PI 3.141592653
typedef double Ldouble;


class MathTool{
public:
	static inline bool equal(Ldouble x, Ldouble y) {
		return fabs(x - y) < Min;
	}
};

#endif // !MathTool_h
