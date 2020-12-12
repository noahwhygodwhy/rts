#ifndef TRIANGLE_TREE_H
#define TRIANGLE_TREE_H

#include <vector>
#include <glm/glm.hpp>
#include "Triangle.hpp"

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
	TriangleTree(vector<Triangle> tIn);
	TriangleTree();
	~TriangleTree();
	Triangle getTriangle(vec2 p);
private:
	axisNode* head;

};

#endif;