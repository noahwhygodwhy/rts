#include "TriangleTree.hpp"
#include <set>
#include "Triangle.hpp"
#include "Shader.hpp"
#include "UsefulStructs.hpp"
#include "glad.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;



axisNode* constructTree(const vector<Triangle>& tris, set<float> xCoords, set<float> yCoords, bool x)
{
	vector<Triangle>* lesserTris = new vector<Triangle>();
	vector<Triangle>* greaterTris = new vector<Triangle>();

	set<float> lesserCoords;
	set<float> greaterCoords;

	float coord = x ? *xCoords.begin() : *yCoords.begin();

	printf("coord is %i%s\n", coord, x ? "x" : "y");

	for (const Triangle& t : tris)
	{
		if (x ? t.mins.x < coord : t.mins.y < coord)
		{
			t.print("less ");
			lesserTris->push_back(t);
		}
		if (x ? t.maxs.x > coord : t.maxs.y > coord)
		{
			t.print("more ");
			greaterTris->push_back(t);
		}
	}


	if (lesserTris->size() == 0 || greaterTris->size() == 0)
	{
		set<float> newXCoords = xCoords;
		set<float> newYCoords = yCoords;
		if (x)
		{
			newXCoords.erase(std::find(newXCoords.begin(), newXCoords.end(), coord));
		}
		else
		{
			newYCoords.erase(std::find(newYCoords.begin(), newYCoords.end(), coord));
		}
		return constructTree(tris, newXCoords, newYCoords, x);
		//todo: return a construct a tree without that xcoord, also the not empty case needs to remove the used up coord too
	}
	axisNodeBranch* toReturn = new axisNodeBranch;
	toReturn->leaf = false;
	toReturn->x = x;
	toReturn->coord = coord;

	if (x)
	{
		for (float f : xCoords)
		{
			if (f != coord)
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
		}
		toReturn->more = constructTree(*greaterTris, greaterCoords, yCoords, !x);
		toReturn->less = constructTree(*lesserTris, lesserCoords, yCoords, !x);
	}
	else//(y)
	{
		for (float f : yCoords)
		{
			if (f != coord)
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
		}
		toReturn->more = constructTree(*greaterTris, xCoords, greaterCoords, !x);
		toReturn->less = constructTree(*lesserTris, xCoords, greaterCoords, !x);
	}
	return toReturn;
}



TriangleTree::TriangleTree(vector<Triangle> tIn, int width, int height)
{
	//printf("triangle tree constructor\n");
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
	setupBuffers(width, height);
}
TriangleTree::TriangleTree()
{
	//printf("triangle tree default constructor\n");
	this->head = new axisNode();
	this->head->leaf = true;
}

TriangleTree::~TriangleTree()
{
}


Triangle TriangleTree::getTriangle(vec2 p)
{
	printf("getting triangle for point %f,%f\n", p.x, p.y);
	axisNode* curr = this->head;
	float coord = 0;
	while (!curr->leaf)
	{
		axisNodeBranch* actualCurr = (axisNodeBranch*)curr;
		printf("On axis: %s\n", actualCurr->x ? "x" : "y");
		float coord = actualCurr->x ? p.x : p.y;
		printf("distinguishing on %f\n", actualCurr->coord);
		curr = (axisNode*)((coord > actualCurr->coord )? actualCurr->more : actualCurr->less);
	}
	vector<Triangle> tris = ((axisNodeLeaf*)curr)->value;
	for (const Triangle& t : tris)
	{
		if (t.contains(p))
		{
			return t;
		}
	}
	return { vec2(0), vec2(0), vec2(0) };
}



vector<Vertex> makeVerts(axisNode* node, int width, int height)
{
	vector<Vertex> vertices;
	float coord = 0;
	if (!node->leaf)
	{
		printf("node is not leaf\n");
		axisNodeBranch* actualCurr = (axisNodeBranch*)node;
		if (actualCurr->x)
		{
			vertices.push_back({ vec2(actualCurr->coord, 0), vec2(0)});
			vertices.push_back({ vec2(actualCurr->coord, height), vec2(0) });
		}
		else
		{
			vertices.push_back({ vec2(0, actualCurr->coord), vec2(0) });
			vertices.push_back({ vec2(width, actualCurr->coord), vec2(0) });
		}
		vector<Vertex> more = makeVerts(actualCurr->more, width, height);
		vector<Vertex> less = makeVerts(actualCurr->more, width, height);
		printf("more: %i, less: %i\n", more.size(), less.size());
		vertices.insert(vertices.end(), more.begin(), more.end());
		vertices.insert(vertices.end(), less.begin(), less.end());
	}
	else
	{
		printf("node is not leaf\n");
	}

	return vertices;
}



void TriangleTree::setupBuffers(int width, int height)
{

	printf("making tt verts\n");
	this->vertices = makeVerts(this->head, width, height);//returning 0 vertices lol

	this->vertices.push_back({ vec2(0), vec2(0) });
	this->vertices.push_back({ vec2(0,500), vec2(0) });
	this->vertices.push_back({ vec2(500,0), vec2(0) });

	printf("there are %i vertices in the triangle tree\n", this->vertices.size());
	for (Vertex v : this->vertices)
	{
		printf("%f, %f\n", v.position.x, v.position.y);
	}


	printf("here1\n");
	glGenVertexArrays(1, &this->VAO);
	printf("here2\n");
	glGenBuffers(1, &this->VBO);
	printf("here3\n");

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void TriangleTree::draw(const Shader& shader)
{


	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

	shader.setBool("outline", false);
	shader.setBool("ignoreAlpha", true);
	shader.setVecThree("tintRatio", vec3(1.0f, 1.0f, 1.0f));
	shader.setVecThree("tint", vec3(1.0f, 0.0f, 0.0f));

	shader.setMatFour("transform", mat4(1));

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	glBindVertexArray(0);
}