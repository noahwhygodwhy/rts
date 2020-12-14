#pragma once
#ifndef MY_MAP_H
#define MY_MAP_H

#include "NavMesh.hpp"
#include "Delaunay.hpp"
#include "UsefulStructs.hpp"
#include "Shader.hpp"
#include <vector>
#include <string> 
#include <glm/glm.hpp>

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
	NavMesh navMesh;

};

//vector<Triangle> generateNavMeshVerts(string inFilePath, string outFilePath);
#endif
