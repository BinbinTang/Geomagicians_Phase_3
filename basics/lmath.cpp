#include "li.h"
#include "lmath.h"

int signDet(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3){	
				return det3D(x1,y1,w1,
					x2,y2,w2,
					x3,y3,w3).sign();
}

int signDet(LongInt x[3][3]){
	return det3D(x[0][0],x[0][1],x[0][2],
		x[1][0],x[1][1],x[1][2],
		x[2][0],x[2][1],x[2][2]).sign();
}

LongInt det3D(LongInt x1, LongInt y1, LongInt w1,
			  LongInt x2, LongInt y2, LongInt w2,
			  LongInt x3, LongInt y3, LongInt w3){
				  LongInt det = x1*y2*w3 + x2*y3*w1 + x3*y1*w2 - x1*y3*w2 - x2*y1*w3 - x3*y2*w1;
				  return det;
}

int signDet4D(LongInt x1, LongInt y1, LongInt w1, LongInt v1,
			LongInt x2, LongInt y2, LongInt w2, LongInt v2,
			LongInt x3, LongInt y3, LongInt w3, LongInt v3,
			LongInt x4, LongInt y4, LongInt w4, LongInt v4){
				LongInt det = -v1*det3D(x2,y2,w2,x3,y3,w3,x4,y4,w4)
					+v2*det3D(x1,y1,w1,x3,y3,w3,x4,y4,w4)
					-v3*det3D(x1,y1,w1,x2,y2,w2,x4,y4,w4)
					+v4*det3D(x1,y1,w1,x2,y2,w2,x3,y3,w3);
				return det.sign();
}

int signDet2D(LongInt ax, LongInt ay, LongInt bx, LongInt by){
	LongInt det = ax*by - ay*bx;
	return det.sign();
}

LongInt square(LongInt x){
	return x*x;
}