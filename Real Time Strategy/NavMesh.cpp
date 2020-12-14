#include "NavMesh.hpp"
#include <unordered_map>
#include <set>
#include <queue>

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



/*
	g-cost:	the sum of the distances between the path points so far
	h-cost: the distance between goal and the closest point from the triangle we're considering to the goal
	f-cost: g+h

*/







vector<vec2> NavMesh::getPath(vec2 start, vec2 end)
{
	
	//unordered_map<Triangle, float> hcost;
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
	Triangle endTri = this->triTree.getTriangle(end);

	unordered_map<vec2, vec2> cameFrom;



	/*struct cmpByStringLength
	{
		bool operator()(const Triangle& a, const Triangle& b) const
		{
			fcost[a] > fcost[b];
		}
	};*/
	auto test = [&](Triangle a, Triangle b) -> bool
	{
		return fcost[a] > fcost[b];
	};
	//decltype([](Triangle lhs, Triangle rhs) { return false; })
	priority_queue <Triangle, vector<Triangle>, decltype(test)> open;


	//priority_queue<vec2> open;
	open.push(startTri);
	
	while (!open.empty())
	{
		Triangle curr = open.top();
		open.pop();
		if (curr == endTri)
		{
			//todo: reconstruct path
			break;
			return;//idk
		}
		for (Triangle* neighbor : this->adjacencySet[curr])
		{
			float tentativeG = gcost[curr] + getCost(curr, neighbor);
			if (tentativeG < gcost[*neighbor])
			{

			}
		}


	}


}
