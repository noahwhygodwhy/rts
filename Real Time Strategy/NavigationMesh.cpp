#include "NavigationMesh.hpp"
#include "Triangle.hpp"




unordered_map<Triangle, vector<Triangle>> constructAdjacencySet(vector<Triangle> tris)
{
	unordered_map<Triangle, vector<Triangle>> toReturn;
	for (const Triangle& t : tris)
	{
		vector<Triangle> adjacentTris;
		for (const Triangle& ot : tris)
		{
			if (!(t == ot))
			{
				if (t.isAdjacent(ot))
				{
					
					adjacentTris.push_back(ot);
				}
			}
		}
		toReturn[t] = adjacentTris;
	}
	return toReturn;
}

NavigationMesh::NavigationMesh(vector<Triangle> tris)
{
	this->triTree = TriangleTree(tris);
	this->adjacencySet = constructAdjacencySet(tris);
}

vector<vec2> NavigationMesh::getPath(vec2 p1, vec2 p2)
{
	//TODO: https://community.monogame.net/t/a-based-pathfinding-using-triangular-navigation-meshes-in-c/11318
}

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::~NavigationMesh()
{
}