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



struct textureAttributes
{
	int x;
	int y;
	string action;
};

namespace std {
	template<>
	struct hash<textureAttributes> {
		inline size_t operator()(const textureAttributes& x) const
		{
			hash<string> stringHasher;
			hash<int> intHasher;

			size_t value = stringHasher(x.action)^intHasher(x.x)^intHasher(x.y); //= your hash computations over x
			return value;
		}
	};

	template<>
	struct equal_to<textureAttributes> {
		inline bool operator()(const textureAttributes& a, const textureAttributes& b) const
		{	
			return a.x == b.x && a.y == b.y && a.action == b.action;
		}
	};
	/*bool operator==(const textureAttributes& a, const textureAttributes& b)
	{
		return a.x == b.x && a.y == b.y && a.action == b.action;
	}*/
}




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
	Entity(vec2 location, int width, int height, Controller c,const unordered_map<textureAttributes, vector<Texture>*>& textures);
	void draw(Shader& shader);
	void tick(float deltaTime);

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
	unordered_map<textureAttributes, vector<Texture>*> textures;
	textureAttributes textureState;
	int textureAnimationStep;
};

#endif