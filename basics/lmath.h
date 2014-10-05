#ifndef LMATHH
#define LMATHH

#include "li.h"

// return 1 as positive, -1 as negative, 0 as zero of the determinant
int signDet(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3);

int signDet(LongInt x[3][3]) ;

int signDet2D(LongInt ax, LongInt ay, LongInt bx, LongInt by);

int signDet4D(LongInt x1, LongInt y1, LongInt w1, LongInt v1,
			LongInt x2, LongInt y2, LongInt w2, LongInt v2,
			LongInt x3, LongInt y3, LongInt w3, LongInt v3,
			LongInt x4, LongInt y4, LongInt w4, LongInt v4);

LongInt det3D(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3);


LongInt square(LongInt x);

#endif