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

vector<int> PointSetArray::twoNearestPoints(int pIdx){
	//we assume we already added pIdx, so pIdx is the last point
	vector<int> result;
	vector<MyPoint> points = myPoints;
	LongInt minDist1, minDist2;
	LongInt dif_x, dif_y, distance;
	int resultP1, resultP2;
	for(int p = 0; p < noPt()-1; p++){
		//calculate dis
		dif_x = myPoints.at(p).x - myPoints.at(pIdx).x;
		dif_y = myPoints.at(p).y - myPoints.at(pIdx).y;
		distance = dif_x * dif_x + dif_y * dif_y;
		if(p==0){
			minDist1 = distance;
			resultP1 = 0;
			continue;
		}
		if(p==1){
			minDist2 = distance;
			resultP2 = 1;
			continue;
		}
		if(distance < minDist1){
			minDist1 = distance;
			resultP1 = p;
			continue;
		}		
		if(distance < minDist2){
			minDist2 = distance;
			resultP2 = p;
			continue;
		}
	}
	result.push_back(resultP1);
	result.push_back(resultP2);

	return result;
}
