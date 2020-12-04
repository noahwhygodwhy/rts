#pragma once

#ifndef SELECTION_BOX_H
#define SELECTION_BOX_H

#include "UsefulStructs.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.hpp"
#include <unordered_map>
#include "Entity.hpp"
#include <unordered_set>

using namespace std;
using namespace glm;


static int SELECTION_ANIMATION_SLOWDOWN_FACTOR = 8;


class SelectionBox
{
public:
	SelectionBox(unordered_map<textureAttributes, vector<Texture>*> textures);
	SelectionBox(){}
	~SelectionBox();
	vector<Vertex> makeSquareVertices(vec2 pointA, vec2 pointB);

	void draw(Shader& shader);
	void tick(vector<Entity*> things, vec2 mousePos);
	bool intersecting(vec2 minA, vec2 maxA, vec2 minB, vec2 maxB);
	vec2 a;
	vec2 b;
	vec3 color;
	bool active;
	void start(vec2 mousePos);
	void stop(vector<Entity*> things, bool shift);
	void stopPrematurely();
	vec2 minPoint;
	vec2 maxPoint;
	void detectClickSelection(vector<Entity*> things, vec2 mousePos, bool shift);
	unordered_set<Entity*> selected;
	unordered_set<Entity*> curSelection;

private:
	unsigned int VAO, VBO, FaceEBO, EdgeEBO;
	vector<Vertex> vertices;
	vector<unsigned int> edgeIndices;
	vector<unsigned int> faceIndices;
	void setupBuffer();
	unordered_map<textureAttributes, vector<Texture>*> textures;
	textureAttributes textureState;
	int textureAnimationStep;
};


#endif