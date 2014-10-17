#ifndef TRISTH
#define TRISTH

#include "pointSetArray.h"
#include <vector>
#include <set>
#include "..\basics\li.h"

/*

  For a triangle abc, if version 0 is abc

  version 0 abc     (v:012)
  version 1 bca		(v:120)
  version 2 cab		(v:201)
  version 3 bac		(v:102)
  version 4 cba		(v:210)
  version 5 acb		(v:021)

  enext cycles   0 > 1 > 2 > 0 ,  5 > 4 > 3 > 5
  sym cycles  i <> (i + 3) % 6

  org  = ver < 3 ? v[ver] : v[(ver+1)%3]
  dest = ver < 3 ? v[(ver+1)%3] : v[ver-3] 

*/



typedef  int OrTri;  // The OrTri data structure for a triangle
typedef  int FIndex; // The index of a triangle Hint: NOT a triangle if it's negative
                     // You should be able to make all the triangle indices to be from 0 to n - 1 (n = number of triangles)

class TriangulateState {
public:
	int step;
	vector<int> linkPoints;
	std::vector<int> bigTriangle;

	bool isDone() { return step == -1; };
};

class Trist;

class TriRecord {
protected:
		int vi_[3];
		OrTri fnext_[6];
		int triIdx;
		bool visible;
	friend Trist;

public:
	bool getVisibility(){ return visible; }
	int* getVertices(){ return vi_; }
	int getIdx(){ return triIdx; }
};

class Trist {
	private : 
		PointSetArray pointSet;
		vector<TriRecord> records;
		int maxTriIdx;
		TriRecord* findTriangle(int tIdx);
		vector<int> bigTriangle;

		set<int> pointsOnTri;
		set<int> lastPointsAddedIdx;
		set<int> lastTrianglesAddedIdx;
		set<int> activeEdge;
		int activePoint;
		bool flipping;

	protected:
		int en_[6];

	public:
		Trist();
		int addPoint(LongInt x, LongInt y);
		int getPoint (int pIndex, LongInt& x1,LongInt& y1); // put the x,y values into x1,y1, and return 1 if the point pIndex exists
		int noPt();                                         // return the number of points

		int noTri(); // return the number of triangles
		int makeTri(int pIndex1,int pIndex2,int pIndex3,bool autoMerge = false); // Add a triangle into the Trist with the three point indices
		// Moreover, automatically establish the fnext pointers to its neigbhours if autoMerge = true

		void delTri(OrTri ef); // Delete a triangle, but you can assume that this is ONLY used by the IP operation
		                    // You may want to make sure all its neighbours are detached (below)
		
		void make3Tri(LongInt x, LongInt y);
		vector<int> make3Tri(int pIdx); //the point already exists

		OrTri enext(OrTri ef);
		OrTri sym(OrTri ef);
		OrTri fnext(OrTri ef);

		void getVertexIdx(OrTri ef, int& pIdx1,int& pIdx2,int& pIdx3); // return the three indices of the three vertices by OrTri
		int org(OrTri ef);  // the index of the first vertex of OrTri, e.g. org(bcd) => b
		int dest(OrTri ef); // the index of the second vertex of OrTri, e.g. org(bcd) => c
		void fmerge(OrTri abc, OrTri abd); // glue two neighbouring triangles, result abd = fnext(abc)
		void fdetach(OrTri abc); // detach triangle abc with all its neighbours (undo fmerge)

		//void incidentTriangles(int ptIndex,int& noOrTri, OrTri* otList); // A suggested function: you may want this function to return all the OrTri
		                                                                 // that are incident to this point
		                                                                 // Ignore this if you don't feel a need
		OrTri inTriangle(int ptIndex); //if returns -1, we are not in any triangle (ptIndex is not the vertex of any triangle)
		vector<int> adjacentTriangles(int pIdx1, int pIdx2); //indexes of triangles wich have pIdx1,pIdx2 as edge
		vector<int> adjacentTriangles(int pIdx); //indexes of triangles wic<h have pIdx as vertex
		
		std::vector< std::pair<OrTri, int> > findNeighbours(TriRecord tri);

		bool isLocallyDelaunay(int pIdx1, int pIdx2);
		vector<int> flipEdge(int pIdx1, int pIdx2);
		void flippingAlg(int pIdx1, int pIdx2);
		void triangulate(); //we assume there is no triangle
		TriangulateState *triangulateByPoint(int pIdx);
		bool triangulateByPointStep(TriangulateState *state);
		void hideBigTriangle(TriangulateState *state);
		void addPointUpdate(LongInt x, LongInt y);
		std::vector<int> getTriIdx();
		void setVisibility(int triIdx, bool visibility);
		std::vector<TriRecord> getTriangles();
		std::vector<MyPoint> getPoints();

		void clearActive() { activeEdge.clear();
						     lastPointsAddedIdx.clear();
							 lastTrianglesAddedIdx.clear(); 
							 activePoint = -1; };
		void setActiveEdge(int pIdx1, int pIdx2) { activeEdge.clear(); activeEdge.insert(pIdx1); activeEdge.insert(pIdx2); };
		void clearActiveEdge() { activeEdge.clear(); };
		bool isPointOnTri(int pIdx) { return pointsOnTri.count(pIdx) == 1; };
		bool isPointLastAdded(int pIdx) { return lastPointsAddedIdx.count(pIdx) == 1; };
		bool isTriangleLastAdded(int tIdx) { return lastTrianglesAddedIdx.count(tIdx) == 1; };
		bool isActiveEdge(int pIdx1, int pIdx2) { return activeEdge.count(pIdx1) == 1 && activeEdge.count(pIdx2) == 1; };
		bool isActivePoint(int pIdx) { return activePoint == pIdx; };
};

#endif
