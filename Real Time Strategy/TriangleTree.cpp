#include "TriangleTree.hpp"
#include <set>
#include "Triangle.hpp"

using namespace glm;
using namespace std;



void* constructTree(vector<Triangle> tris, set<vec2> points, bool x)
{
	vec2 p = *points.begin();
	vector<Triangle> sortedTris[3];
	vector<Triangle> sortedPoints[3];

	for (const Triangle& t : tris)
	{
		if (x ? t.getEnvelope().first.x < p.x : t.getEnvelope().first.y < p.y) //TODO: wrong side for comparison
		{
			sortedTris->push_back(t);
		}
		else if (x ? t.getEnvelope().second.x > p.x : t.getEnvelope().second.y > p.y)
		{

		}
	}


	if (tris.size() == 1)
	{
		return &tris[0];
	}
	
}



TriangleTree::TriangleTree(vector<Triangle> tIn)
{
	set<vec2> points;
	for (Triangle t : tIn)
	{
		for (vec2 p : t.points)
		{
			points.insert(p);//no duplicates cause set
		}
	}
	for (vec2 p : points)
	{

	}

}

TriangleTree::~TriangleTree()
{
}