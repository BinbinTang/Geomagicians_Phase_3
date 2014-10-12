#include <iostream>
#include "pointSet.h"
#include "lmath.h"

using namespace std;

int PointSet::addPoint(LongInt x1, LongInt y1){
	struct MyPoint thisPoint;
	thisPoint.x = x1;
	thisPoint.y = y1;
	thisPoint.z = 0;
	thisPoint.visible = true;
	myPoints.push_back(thisPoint);
	return myPoints.size()-1;
}

int PointSet::inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	MyPoint a = this->myPoints.at(p1Idx);
	MyPoint b = this->myPoints.at(p2Idx);
	MyPoint c = this->myPoints.at(p3Idx);
	MyPoint p = this->myPoints.at(pIdx);

	int det1 = signDet(a.x, a.y, 1,
		b.x, b.y, 1,
		c.x, c.y,1);
	int det2 = signDet(a.x - p.x, a.y - p.y, square((a.x - p.x))+square(a.y - p.y),
		b.x - p.x, b.y - p.y, square(b.x - p.x)+square(b.y - p.y),
		c.x - p.x, c.y - p.y, square(c.x - p.x)+square(c.y - p.y));
	return det1*det2;
}

int PointSet::sameSide(MyPoint p1, MyPoint p2, MyPoint a, MyPoint b){
	return signDet2D(b.x-a.x, b.y-a.y, p1.x - a.x, p1.y - a.y)*
		signDet2D(b.x-a.x, b.y-a.y, p2.x - a.x, p2.y - a.y);
}

int PointSet::inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	MyPoint a = this->myPoints.at(p1Idx);
	MyPoint b = this->myPoints.at(p2Idx);
	MyPoint c = this->myPoints.at(p3Idx);
	MyPoint p = this->myPoints.at(pIdx);

	if(sameSide(p,a,b,c)==1 && sameSide(p,b,a,c)==1 && sameSide(p,c,a,b)==1){
		return 1;
	}else if(sameSide(p,a,b,c)==-1 || sameSide(p,b,a,c)==-1 || sameSide(p,c,a,b)==-1) {
		return -1;
	}else{
		return 0;
	}
				
}

int PointSet::turnLeft(int p1Idx, int p2Idx, int p3Idx){
	MyPoint a = this->myPoints.at(p1Idx);
	MyPoint b = this->myPoints.at(p2Idx);
	MyPoint c = this->myPoints.at(p3Idx);
	return signDet2D(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
}

vector<int> PointSet::constructCircumTri(){
	vector<int> points;
	vector<LongInt> minMaxVal = minMax();
	LongInt offset = 5;
	LongInt min_x = minMaxVal.at(0) - offset;
	LongInt min_y = minMaxVal.at(1) - offset;
	LongInt max_x = minMaxVal.at(2) + offset;
	LongInt max_y = minMaxVal.at(3) + offset;

	points.push_back(addPoint(min_x - (max_y - min_y), min_y));
	points.push_back(addPoint(max_x, min_y));
	points.push_back(addPoint(max_x, max_y + max_x - min_x));
	cout << "Added points: (" << (min_x - (max_y - min_y)).printOut().c_str() << ", " << min_y.printOut().c_str() << "), (" 
		<< max_x.printOut().c_str() << ", " << min_y.printOut().c_str() << "), (" << max_x.printOut().c_str() << ", " << (max_y + max_x - min_x).printOut().c_str() << ")" << endl;
	
	setVisibility(points.at(0), false);
	setVisibility(points.at(1), false);
	setVisibility(points.at(2), false);
	return points;
}

vector<LongInt> PointSet::minMax(){
	vector<LongInt> result;
	int tempOffX = 202;
	int tempOffY = 203;
	LongInt min_x(-tempOffX), min_y(-tempOffY), max_x(1000 - tempOffX), max_y(700 - tempOffY);

	/*
	for(vector<MyPoint>::iterator it = myPoints.begin(); it != myPoints.end(); ++it){
		p = *it;
		if(min_x > p.x){
			min_x = p.x;
		}
		if(max_x < p.x){
			max_x = p.x;
		}
		if(min_y > p.y){
			min_y = p.y;
		}
		if(max_y < p.y){
			max_y = p.y;
		}
	}
	*/
	result.push_back(min_x);
	result.push_back(min_y);
	result.push_back(max_x);
	result.push_back(max_y);
	return result;
}

void PointSet::setVisibility(int pIdx, bool visibility){
	myPoints.at(pIdx).visible = visibility;
}

vector<MyPoint> PointSet::getPoints(){
	return myPoints;
}