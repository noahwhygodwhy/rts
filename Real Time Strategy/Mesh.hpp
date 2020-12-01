#pragma once


#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad.h"
#include <GLFW/glfw3.h>

#include "Shader.hpp"

using namespace std;
using namespace glm;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

struct Texture {
	unsigned int id = 0;
	string type;
	string path;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void draw(Shader& shader, glm::mat4 transform);
	~Mesh();
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<Vertex>* getVertices();
private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};


#endif