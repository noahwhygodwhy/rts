#include "NavMesh.hpp"
#include <unordered_map>
#include <set>
#include <queue>
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;






unordered_map<Triangle, vector<Triangle>> constructAdjacencySet(vector<Triangle> tris)
{
	unordered_map<Triangle, vector<Triangle>> toReturn;
	for (const Triangle& t : tris)
	{
		t.print("finding adjancet tris for ");
		vector<Triangle> adjacentTris;
		for (Triangle& ot : tris)
		{
			//ot.print();
			if (t.isAdjacent(ot))
			{
				//printf("### adj\n");
				adjacentTris.push_back(ot);
			}
			else
			{
				//printf("not adj\n");
			}
		}
		toReturn[t] = adjacentTris;
	}
	return toReturn;
}

NavMesh::NavMesh(vector<Triangle> tris, unordered_set<Edge> fedges, int width, int height)
{
	this->tris = tris;
	printf("Navmesh triangles:\n");
	for (const Triangle& t : this->tris)
	{
		t.print();
	}
	printf("end of navmesh triangles\n");
	this->triTree = TriangleTree(tris, width, height);
	this->adjacencySet = constructAdjacencySet(this->tris);
	this->fedges = fedges;
	setupBuffers();
}

NavMesh::NavMesh()
{
	this->tris = vector<Triangle>();
	this->triTree = TriangleTree();
	this->adjacencySet = constructAdjacencySet(this->tris);
	this->fedges = unordered_set<Edge>();
	//setupBuffers();
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
	float g2 = abs(distance(start, end) - getHCost(tri, end));//distance(start, end) - curr.hcost
	float g3 = abs(gcost.at(parent) + (getHCost(parent, end) - getHCost(tri, end)));//parent.gcost + (parent.h - curr.h)
	float g4 = abs(gcost.at(parent));//parent.gcost + (nearest point on parent entry edge, and nearest point of curr)//ooh this one seems good

	//printf("G1: %f\n", g1);
	//printf("G2: %f\n", g2);
	//printf("G3: %f\n", g3);
	//printf("G4: %f\n", g4);



	return std::max(std::max(g1, g2), std::max(g3, g4));

	//TODO:
	//distance between startand nearest point on ENTRY EDGE(? ) of curr
	//distance(start, end) - curr.hcost
	//parent.gcost + (parent.h - curr.h)
	//parent.gcost + (nearest point on parent entry edge, and nearest point of curr)//ooh this one seems good
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



bool intersectsListOfEdges(const Edge& e, const unordered_set<Edge>& fedges)
{
	for (const Edge& f : fedges)
	{
		if (e.intersects(f))
		{
			return true;
		}
	}
	return false;
}

vector<vec2> NavMesh::reconstructPath(vec2 start, vec2 end, const unordered_map<vec2, vec2>& cameFrom, const unordered_set<Edge>& fedges)
{

	printf("start %f, %f\n", start.x, start.y);
	printf("end %f, %f\n", end.x, end.y);
	printf("there are %i nodes\n", cameFrom.size());
	for (pair<vec2, vec2> x : cameFrom)
	{
		printf("%f,%f came from %f, %f\n", x.first.x, x.first.y, x.second.x, x.second.y);
		//this->triTree.getTriangle(x.second).print();
	}
	//printf("%f, %f\n", end.x, end.y);
	vector<vec2> nodes;
	nodes.push_back(end);
	try
	{
		//vec2 curr = cameFrom.at(end);
		//printf("%f, %f\n", curr.x, curr.y);
		vec2 curr = cameFrom.at(end);
		while (true)
		{
			printf("curr: %f,%f\n", curr.x, curr.y);
			nodes.push_back(curr);
			curr = cameFrom.at(curr);
		}
	}
	catch (exception e)
	{
		printf("exception\n");
	}

	if (nodes.size() > 2)
	{
		auto nodeIter = nodes.begin();
		while (nodeIter != nodes.end() - 2)
		{
			Edge ac = { *nodeIter, *(nodeIter+2) };
			bool acIntersect = false;
			for (Edge f : this->fedges) //maybe too expensive
			{
				acIntersect = ac.intersects(f) || acIntersect;
				if(acIntersect)
				{
					Edge ad = { *nodeIter, f.points[0] };
					Edge dc = { *(nodeIter+2), f.points[0] };
					if (!intersectsListOfEdges(ad, fedges) && !intersectsListOfEdges(dc, fedges))
					{
						*(nodeIter + 1) = f.points[0];
						break;
					}
					else
					{
						Edge ae = { *nodeIter, f.points[1] };
						Edge ec = { *(nodeIter + 2), f.points[1] };
						if (!intersectsListOfEdges(ae, fedges) && !intersectsListOfEdges(ec, fedges))
						{
							*(nodeIter + 1) = f.points[1];
							break;
						}
					}
				}
			}
			if (!acIntersect)
			{
				nodes.erase(nodeIter+1);
			}
			else
			{
				nodeIter++;//is this and the breaks in the right location?
			}
		}
	}




	return nodes;
}

vector<vec2> NavMesh::getPath(vec2 start, vec2 end)
{
	printf("getting path from %f,%f to %f,%f\n", start.x, start.y, end.x, end.y);

	printf("Navmesh triangles:\n");
	for (const Triangle& t : this->tris)
	{
		t.print();
	}
	printf("end of navmesh triangles\n");
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
	startTri.print("Start Tri:");
	printf("center of startTri: %f,%f\n", startTri.geoCenter.x, startTri.geoCenter.y);
	gcost[startTri] = 0;
	fcost[startTri] = 0;
	Triangle endTri = this->triTree.getTriangle(end);

	unordered_map<vec2, vec2> cameFrom;

	cameFrom[startTri.geoCenter] = start;

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
		curr.print("======curr: ");
		open.pop();
		openContents.erase(find(openContents.begin(), openContents.end(), curr));
		if (curr == endTri)
		{
			cameFrom[end] = curr.geoCenter;
			printf("reconstructing\n");
			return reconstructPath(start, end, cameFrom, fedges);
			//return reconstruct(start, end, cameFrom);//todo:
		}
		printf("has %i neighbors\n", this->adjacencySet[curr].size());
		for (Triangle neighbor : this->adjacencySet.at(curr))
		{
			neighbor.print("neighbor: ");
			vec2 cameFromPoint = curr.geoCenter; //TODO: need to calculate this https://raygun.com/blog/`-development-triangulated-spaces-part-2/
			float tentativeG = gcost[curr] + getGCost(curr, neighbor, start, end, gcost);
			printf("tentative g: %f\n", tentativeG);
			printf("neighbor g: %f\n", gcost[neighbor]);
			if (tentativeG < gcost[neighbor])
			{
				printf("cheaper\n");
				cameFrom[neighbor.geoCenter] = curr.geoCenter; //~~maybe~~definetly not right
				gcost[neighbor] = tentativeG;
				fcost[neighbor] = gcost[neighbor] + getHCost(neighbor, end);
				printf("does it exist in open yet? %i\n", std::count(openContents.begin(), openContents.end(), neighbor));
				if (std::count(openContents.begin(), openContents.end(), neighbor) == 0)
				{
					printf("pushing into open\n");
					openContents.push_back(neighbor);
					open.push(neighbor);
				}

			}
			else
			{
				printf("expensive\n");
			}
		}
	}
	printf("failure to find a path\n");
	return vector<vec2>();//failure to find a path returns an empty vector
}

vector<Vertex> trisToLineVerts(const vector<Triangle>& tris)
{
	vector<Vertex> verts;
	for (const Triangle& t : tris)
	{
		for (int i = 0; i < 3; i++)
		{
			verts.push_back({ t.points[i], vec2(0) });
			verts.push_back({ t.points[(i+1)%3], vec2(0) });
		}
	}
	return verts;
}


void NavMesh::setupBuffers()
{
	this->vertices = trisToLineVerts(this->tris);

	/*for (Vertex v : this->vertices)
	{
		printf("%f, %f\n", v.position.x, v.position.y);
	}

	printf("nav mesh buffering %i vertices\n", vertices.size());*/

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void NavMesh::draw(const Shader& shader)
{
	//this->triTree.draw(shader);

	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

	shader.setBool("outline", false);
	shader.setBool("ignoreAlpha", true);
	shader.setVecThree("tintRatio", vec3(1.0f));
	shader.setVecThree("tint", vec3(1.0f, 0.0f, 1.0f));

	shader.setMatFour("transform", mat4(1));

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glDrawArrays(GL_LINES, 0, this->vertices.size());

	glBindVertexArray(0);
}
