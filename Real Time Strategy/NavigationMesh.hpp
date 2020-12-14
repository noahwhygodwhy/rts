#include <unordered_map>
#include "TriangleTree.hpp"
#include "Triangle.hpp"
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class NavigationMesh
{
public:
	NavigationMesh();
	NavigationMesh(vector<Triangle> tris);
	~NavigationMesh();
	vector<vec2> getPath(vec2 p1, vec2 p2);
private:
	vector<Triangle> tris;
	TriangleTree triTree;
	unordered_map<Triangle, vector<Triangle>> adjacencySet;
};
