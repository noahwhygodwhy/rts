#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "glm/glm.hpp"
#include "Controller.hpp"
#include <unordered_map>
#include <vector>
#include <string>
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

class Entity
{
public:
	Entity(vec2 location, int width, int height, Controller c,const unordered_map<string, vector<Texture>*>& textures);
	void draw(Shader& shader, mat4 transform);

	~Entity();

private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	void setupBuffer();
	vec2 location;
	ivec2 orientation;
	float speed;
	Controller controller;
	unordered_map<string, vector<Texture>*> textures;
	string state;
};

#endif