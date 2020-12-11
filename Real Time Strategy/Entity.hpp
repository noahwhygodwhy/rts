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

enum TargetType
{
	NONE,
	LOCATION,
	ENTITY
};


class Entity
{
public:
	Entity(vec2 location, vec2 dims, float hitboxRadius, Controller c,const unordered_map<textureAttributes, vector<Texture>*>& textures);
	void draw(Shader& shader);
	void tick(float deltaTime);
	void setTarget(vec2 targetLocation);
	void setTarget(Entity* targetEntity);
	~Entity();
	void setOrientation(vec2 newO);
	ivec2 orientation;
	vec2 location;
	vec2 dims;
	float hitboxRadius;
	bool selected;
	int id;



	void walkToLocation(float deltaTime);

	bool operator==(const Entity& other)const
	{
		return this->id == other.id;
	}

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
	TargetType targetType = TargetType::NONE;
	void* target = 0;

};

#endif