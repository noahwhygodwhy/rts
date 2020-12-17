#ifndef TRIANGLE_TREE_H
#define TRIANGLE_TREE_H

#include <vector>
#include <glm/glm.hpp>
#include "Triangle.hpp"
#include "Shader.hpp"
#include "UsefulStructs.hpp"

using namespace std;
using namespace glm;


/*class axisNode
{
	axisNode()
	{

	}
	~axisNode()
	{

	}
public:
	bool x;
	int coord;
	void* more;
	void* less;
private:
};*/

struct axisNode
{
	bool leaf;
};

struct axisNodeLeaf : axisNode
{
	vector<Triangle> value;
};
struct axisNodeBranch : axisNode
{
	bool x;
	int coord;
	axisNode* more;
	axisNode* less;
};


class TriangleTree
{
public:
	TriangleTree(vector<Triangle> tIn, int width = 0, int height = 0);
	TriangleTree();
	~TriangleTree();
	Triangle getTriangle(vec2 p);
	void setupBuffers(int width, int height);
	void draw(const Shader& shader);

private:
	axisNode* head;
	unsigned int VAO, VBO;
	vector<Vertex> vertices;

};

#endif;