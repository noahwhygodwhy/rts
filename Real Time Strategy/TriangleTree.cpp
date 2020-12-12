#include "TriangleTree.hpp"
#include <set>
#include "Triangle.hpp"

using namespace glm;
using namespace std;



axisNode* constructTree(const vector<Triangle>& tris, set<float> xCoords, set<float> yCoords, bool x)
{
	/*if (tris.size() == 1)
	{
		return &tris[0];
	}*/




	vector<Triangle>* lesserTris = new vector<Triangle>();
	vector<Triangle>* greaterTris = new vector<Triangle>();
	//vector<Triangle> innerTris;

	set<float> lesserCoords;
	set<float> greaterCoords;

	float coord = x ? *xCoords.begin() : *yCoords.begin();

	for (const Triangle& t : tris)
	{
		if (x ? t.getEnvelope().first.x < coord : t.getEnvelope().first.y < coord)
		{
			lesserTris->push_back(t);
		}
		if (x ? t.getEnvelope().second.x > coord : t.getEnvelope().second.y > coord)
		{
			greaterTris->push_back(t);
		}
	}
	if (lesserTris->size() == 0)
	{
		axisNodeLeaf* a = new axisNodeLeaf;
		a->value = *greaterTris;
		return a;
	}
	if (greaterTris->size() == 0)
	{
		axisNodeLeaf* a = new axisNodeLeaf;
		a->value = *lesserTris;
		return a;
	}
	axisNodeBranch* toReturn;
	toReturn->leaf = false;
	toReturn->x = x;
	toReturn->coord = coord;

	if (x)
	{
		for (float f : xCoords)
		{
			if (f > coord)
			{
				greaterCoords.insert(f);
			}
			else
			{
				lesserCoords.insert(f);
			}
		}
		toReturn->more = constructTree(*greaterTris, greaterCoords, yCoords, !x);
		toReturn->less = constructTree(*lesserTris, lesserCoords, yCoords, !x);
	}
	else//(y)
	{
		for (float f : yCoords)
		{
			if (f > coord)
			{
				greaterCoords.insert(f);
			}
			else
			{
				lesserCoords.insert(f);
			}
		}
		toReturn->more = constructTree(*greaterTris, xCoords, greaterCoords, !x);
		toReturn->less = constructTree(*lesserTris, xCoords, greaterCoords, !x);
	}
	return toReturn;
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
	this->head = constructTree(tIn, xcoords, ycoords, true);
}
TriangleTree::TriangleTree()
{
	this->head = new axisNode;
}

TriangleTree::~TriangleTree()
{
}

Triangle TriangleTree::getTriangle(vec2 p)
{
	axisNode* curr = this->head;
	float coord = 0;
	while (curr->leaf != true)
	{

		axisNodeBranch* actualCurr = (axisNodeBranch*)curr;
		float coord = actualCurr->x ? p.x : p.y;
		curr = (axisNode*)((coord > actualCurr->coord )? actualCurr->more : actualCurr->less);
	}
	vector<Triangle> tris = ((axisNodeLeaf*)curr)->value;
	for (const Triangle& t : tris)
	{
		if (inTriangle(t, p))
		{
			return t;
		}
	}
	return { vec2(0), vec2(0), vec2(0) };
}