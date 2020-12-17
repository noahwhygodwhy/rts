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

NavMesh::NavMesh(vector<Triangle> tris, unordered_set<Edge> fedges)
{
	this->tris = tris;
	this->triTree = TriangleTree(tris);
	this->adjacencySet = constructAdjacencySet(tris);
	this->fedges = fedges;
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


float getHCost(const Triangle& tri, vec2 end)
{
	return distance(tri.closestPoint(end), end);
}


float getGCost(const Triangle& parent, const Triangle& tri, vec2 end, vec2 start, const unordered_map<Triangle, float>& gcost)
{

	float g1 = 0.0f;//TODO: distance between startand nearest point on ENTRY EDGE(? ) of curr
	float g2 = distance(start, end) - getHCost(tri, end);//distance(start, end) - curr.hcost
	float g3 = gcost.at(parent) + (getHCost(parent, end) - getHCost(tri, end));//parent.gcost + (parent.h - curr.h)
	float g4 = gcost.at(parent);//parent.gcost + (nearest point on parent entry edge, and nearest point of curr)//ooh this one seems good





	return std::max(std::max(g1, g2), std::max(g3, g4));

	//TODO:
	//distance between startand nearest point on ENTRY EDGE(? ) of curr
	//distance(start, end) - curr.hcost
	//parent.gcost + (parent.h - curr.h)
	//parent.gcost + (nearest point on parent entry edge, and nearest point of curr)//ooh this one seems good
}

bool counterClockwise(vec2 a, vec2 b, vec2 c)
{
	return (b.x - a.x) * (c.y - a.y) > (c.x - a.x) * (b.y - a.y);
}

//does not account for colinear lines, but we don't care
bool linesIntersect(vec2 a, vec2 b, vec2 c, vec2 d)
{
	if (a == c || a == d || b == c || b == d) //cause we're ok with tips touching
	{
		return false;
	}
	return counterClockwise(a, c, d) != counterClockwise(b, c, d) && counterClockwise(a, b, c) != counterClockwise(a, b, d);
}



vector<vec2> reconstructPath(vec2 start, vec2 end, const unordered_map<vec2, vec2>& cameFrom, const unordered_set<Edge>& fedges)
{
	vector<vec2> nodes;
	try
	{
		vec2 curr = cameFrom.at(end);
		while (true)
		{
			nodes.push_back(curr);
		}
	}
	catch (exception e)
	{
	}
	return nodes;
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
			return reconstructPath(start, end, cameFrom, fedges);
			//return reconstruct(start, end, cameFrom);//todo:
		}
		for (Triangle* neighbor : this->adjacencySet[curr])
		{
			vec2 cameFromPoint = curr.geoCenter; //TODO: need to calculate this https://raygun.com/blog/game-development-triangulated-spaces-part-2/
			float tentativeG = gcost[curr] + getGCost(curr, *neighbor, start, end, gcost);
			if (tentativeG < gcost[*neighbor])
			{
				cameFrom[cameFromPoint] = curr.geoCenter; //~~maybe~~definetly not right
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
