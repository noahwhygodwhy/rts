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


vec2 closestCorner(const Triangle& t, const vec2& p)
{
	vec2 d1 = p - t.points[0];
	vec2 d2 = p - t.points[1];
	vec2 d3 = p - t.points[2];

	float m1 = glm::length(d1);
	float m2 = glm::length(d2);
	float m3 = glm::length(d3);

	float min = std::min(m1, std::min(m2, m3));
	if (min == m1)
	{
		return vec2(t.points[0]);
	}
	if (min == m2)
	{
		return vec2(t.points[1]);
	}
	return vec2(t.points[2]);
	
}




float getGCost(const Triangle& curr, const Triangle& neighbor, vec2* closestPoint)
{
	//TODO:
}
float getHCost(const Triangle& curr, vec2 end)
{
	return distance(curr.closestPoint(end), end);
}

bool counterClockwise(vec2 a, vec2 b, vec2 c)
{
	return (b.x - a.x) * (c.y - a.y) > (c.x - a.x) * (b.y - a.y);
}

//does not account for colinear lines, but we don't care
bool linesIntersect(vec2 a, vec2 b, vec2 c, vec2 d)
{
	return counterClockwise(a, c, d) != counterClockwise(b, c, d) && counterClockwise(a, b, c) != counterClockwise(a, b, d);
}


vector<vec2> NavMesh::getPath(vec2 start, vec2 end)
{
	/**/
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


	auto comparerer = [&fcost](Triangle a, Triangle b)
	{
		return fcost[a] > fcost[b];
	};
	priority_queue <Triangle, vector<Triangle>, decltype(comparerer)> open(comparerer);

	vector<Triangle> openContents;

	open.push(startTri);
	openContents.push_back(startTri);
	while (!open.empty())
	{
		Triangle curr = open.top();
		open.pop();
		openContents.erase(find(openContents.begin(), openContents.end(), curr));
		if (curr == endTri)
		{
			//return reconstruct(start, end, cameFrom);//todo:
		}
		for (Triangle* neighbor : this->adjacencySet[curr])
		{
			vec2 pointOnNeighbor;
			float tentativeG = gcost[curr] + getGCost(curr, *neighbor, &pointOnNeighbor);
			if (tentativeG < gcost[*neighbor])
			{
				cameFrom[pointOnNeighbor] = curr.geoCenter; //maybe not right
				gcost[*neighbor] = tentativeG;
				fcost[*neighbor] = gcost[*neighbor] + getHCost(*neighbor, end);
				if (std::count(openContents.begin(), openContents.end(), *neighbor) == 0)
				{
					openContents.push_back(*neighbor);
					open.push(*neighbor);
				}

			}
		}
	}
	return vector<vec2>();//failure to find a path returns an empty vector
}
