#ifndef TRIANGLE_TREE_H
#define TRIANGLE_TREE_H

#include <vector>
#include <glm/glm.hpp>
#include "Triangle.hpp"
#include "Shader.hpp"
#include "UsefulStructs.hpp"

using namespace std;
using namespace glm;






namespace std
{
	template <>
	struct hash<Vertex>
	{
		size_t operator()(const Vertex k) const
		{
			hash<float> fh;
			return fh(k.position.x) ^ (fh(k.position.y) << 1) ^ (fh(k.texCoords.x) << 2) ^ (fh(k.texCoords.y) << 3);
		}
	};

}
namespace std
{
	template <>
	struct hash<pair<Vertex, Vertex>>
	{
		size_t operator()(const pair<Vertex, Vertex> k) const
		{
			hash<Vertex> vh;
			return vh(k.first) ^ (vh(k.second) << 1);
		}
	};

	/*template <>
	struct hash<pair<Vertex, Vertex>>
	{
		size_t operator()(const pair<Vertex, Vertex> k) const
		{
			hash<Vertex> vh;
			return vh(k.first) ^ (vh(k.second) << 1);
		}
	};*/
}




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
	float coord;
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