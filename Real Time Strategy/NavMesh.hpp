#ifndef NAV_MESH_H
#define NAV_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include "TriangleTree.hpp"
#include "Triangle.hpp"
#include <unordered_map>

using namespace std;
using namespace glm;

class NavMesh
{
public:
	NavMesh();
	NavMesh(vector<Triangle> tris);
	~NavMesh();

private:

	vector<Triangle> tris;
	TriangleTree triTree;
	unordered_map<Triangle, vector<Triangle*>> adjacencySet;
};


#endif