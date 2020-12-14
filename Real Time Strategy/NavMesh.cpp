#include "NavMesh.hpp"
#include <unordered_map>

using namespace std;
using namespace glm;

unordered_map<Triangle, vector<Triangle*>> constructAdjacencySet(vector<Triangle> tris)
{
	unordered_map<Triangle, vector<Triangle*>> toReturn;
	for (const Triangle& t : tris)
	{
		vector<Triangle*> adjacentTris;
		for (Triangle& ot : tris)
		{
			if (!(t == ot))
			{
				if (t.isAdjacent(ot))
				{

					adjacentTris.push_back(&ot);
				}
			}
		}
		toReturn[t] = adjacentTris;
	}
	return toReturn;
}

NavMesh::NavMesh(vector<Triangle> tris)
{
	this->tris = tris;
	this->triTree = TriangleTree(tris);
	this->adjacencySet = constructAdjacencySet(tris);
}

NavMesh::NavMesh()
{
}

NavMesh::~NavMesh()
{
}