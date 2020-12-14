#pragma once
#ifndef MAP_H
#define MAP_H

#include "Delaunay.hpp"
#include "UsefulStructs.hpp"
#include "Shader.hpp"
#include <vector>
#include <string> 
#include <glm/glm.hpp>
#include "NavigationMesh.hpp"

using namespace std;
using namespace glm;


class Map
{
public:
	Map(string path, vec2 dims);
	Map() {}
	void draw(Shader& shader);

	vec2 dims;
	~Map();

private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	void setupBuffer();
	Texture texture;
	NavigationMesh navMesh;

};

//vector<Triangle> generateNavMeshVerts(string inFilePath, string outFilePath);
#endif
