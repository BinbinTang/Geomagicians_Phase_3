#include "pointSetArray.h"

int PointSetArray::getPoint (int pIndex, LongInt& x1,LongInt& y1) // put the x,y values into x1,y1, and return 1 if the point pIndex exists
{
	struct MyPoint point = myPoints.at(pIndex);
	x1 = point.x;
	y1 = point.y;

	return 1;
}

int PointSetArray::noPt()                                        // return the number of points
{
	return myPoints.size();
}
void PointSetArray::eraseAllPoints()                            // erase all points in this array
{
	myPoints.clear();
}
void PointSetArray::eraseThreeLastPoints(){
	if(myPoints.size() >= 3){
		myPoints.erase(myPoints.end()-3, myPoints.end());
	}
}