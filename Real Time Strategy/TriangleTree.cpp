#include "TriangleTree.hpp"
#include <set>
#include "Triangle.hpp"

using namespace glm;
using namespace std;



void* constructTree(vector<Triangle> tris, set<float> xcoords, set<float> ycoords, bool x)
{
	if (tris.size() == 1)
	{
		return &tris[0];
	}
	axisNode toReturn;
	toReturn.x = x;
	float coord = x?*xcoords.begin():*ycoords.begin();
	toReturn.coord = coord;

	vector<Triangle> lesserTris;
	vector<Triangle> greaterTris;
	vector<Triangle> innerTris;

	set<vec2> lesserPoints;
	set<vec2> innerPoints;
	set<vec2> greaterPoints;

	for (const Triangle& t : tris)
	{
		if (x ? t.getEnvelope().second.x < coord : t.getEnvelope().second.y < coord)
		{
			lesserTris.push_back(t);
		}
		else if (x ? t.getEnvelope().first.x > coord : t.getEnvelope().first.y > coord)
		{
			greaterTris.push_back(t);
		}
		else
		{
			innerTris.push_back(t);
		}
	}
	//TODO: possility of all things being in innerTris without tri's size being 1..?
	
	auto poi = points.begin();
	poi++;
	while (poi != points.end())
	{

	}
	
	toReturn.more = constructTree(greaterTris, greaterPoints, !x);
	toReturn.less = constructTree(lesserTris, lesserPoints, !x);
	toReturn.on = constructTree(innerTris, innerPoints, !x);


	
}



TriangleTree::TriangleTree(vector<Triangle> tIn)
{
	set<float> xcoords;
	set<float> ycoords;
	for (Triangle t : tIn)
	{
		for (vec2 p : t.points)
		{
			xcoords.insert(p.x);
			ycoords.insert(p.y);
		}
	}
	constructTree(tIn, xcoords, ycoords, true);

}

TriangleTree::~TriangleTree()
{
}