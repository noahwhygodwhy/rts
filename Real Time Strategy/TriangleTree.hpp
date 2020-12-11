#ifndef TRIANGLE_TREE_H
#define TRIANGLE_TREE_H

#include <glm/glm.hpp>
#include "Triangle.hpp"

using namespace std;
using namespace glm;

struct axisNode
{
	bool x;
	int coord;
	void* more;
	void* less;
	void* on;
};


class TriangleTree
{
public:
	TriangleTree(vector<Triangle> tIn);
	~TriangleTree();
	Triangle getTriangle(vec2 p);
private:
	axisNode head;

};

#endif;