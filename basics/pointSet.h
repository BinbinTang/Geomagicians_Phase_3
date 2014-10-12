#ifndef POINTSETH
#define POINTSETH

#include <vector>
#include "li.h"
#include "lmath.h"

using namespace std;

struct MyPoint {
	LongInt x;
	LongInt y;
	LongInt z;
	bool visible;
};

class PointSet {
protected:
	vector<MyPoint> myPoints;
	int static sameSide(MyPoint p1, MyPoint p2, MyPoint a, MyPoint b);
public:
	int addPoint(LongInt x1,LongInt y1); 
	// add an ith(according to the adding sequence)  point (x1,y1), return i.

	int inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx);
	// return 1 if the point p is inside the triangle p1p2p3, by reference of the indices, -1 if outside, 0 if degenerate

	int inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx);
	// return 1 if the point p is inside the circumcircle of p1p2p3, -1 if outside, 0 if degenerate

	int turnLeft(int p1Idx, int p2Idx, int p3Idx);

	vector<int> constructCircumTri(); //adds the 3 points into the point set
	vector<LongInt> minMax(); //first element : min(x), 2nd : min(y), 3rd: max(x), 4th: max(y)

	void setVisibility(int pIdx, bool visibility);
	vector<MyPoint> getPoints();
};

#endif