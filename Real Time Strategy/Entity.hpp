#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "UsefulStructs.hpp"
#include "glm/glm.hpp"
#include "Controller.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include "Shader.hpp"


using namespace std;
using namespace glm;

static int ANIMATION_SLOWDOWN_FACTOR = 8;


class Entity
{
public:
	Entity(vec2 location, int width, int height, Controller c,const unordered_map<textureAttributes, vector<Texture>*>& textures);
	void draw(Shader& shader);
	void tick(float deltaTime);

	~Entity();
	void setOrientation(vec2 newO);
	ivec2 orientation;
	vec2 location;
private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	void setupBuffer();
	float speed;
	Controller controller;
	unordered_map<textureAttributes, vector<Texture>*> textures;
	textureAttributes textureState;
	int textureAnimationStep;
	vector<Vertex> makeSquareVertices(float width, float height);
};

#endif