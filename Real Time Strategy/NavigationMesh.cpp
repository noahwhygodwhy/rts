
#include "NavigationMesh.hpp"

#include "Triangle.hpp"
#include <glm/glm.hpp>
#include <set>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace glm;

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

/*NavigationMesh::NavigationMesh(vector<Triangle> tris)
{
	this->tris = tris;
	this->triTree = TriangleTree(tris);
	this->adjacencySet = constructAdjacencySet(tris);
}*/



/*
	g-cost:	the sum of the distances between the path points so far
	h-cost: the distance between goal and the closest point from the triangle we're considering to the goal
	f-cost: g+h

*/


/*vector<vec2> NavigationMesh::getPath(vec2 start, vec2 end)
{
	/*
	unordered_map<Triangle, float> hcost;
	unordered_map<Triangle, float> gcost;
	unordered_map<Triangle, float> fcost; 

	for (const Triangle& t : this->tris)
	{
		gcost[t] = INFINITY;
	}
	for (const Triangle& t : this->tris)
	{
		fcost[t] = INFINITY;
	}
	Triangle startTri = this->triTree.getTriangle(start);
	gcost[startTri] = 0;
	fcost[startTri] = 0;

	unordered_map<vec2, vec2> cameFrom;
	set<vec2> open;
	open.insert(start);

	while (!open.empty())
	{

	}
	

}*/
