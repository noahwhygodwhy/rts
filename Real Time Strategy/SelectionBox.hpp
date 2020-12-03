#ifndef SELECTION_BOX_H
#define SELECTION_BOX_H

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "UsefulStructs.hpp"
#include <vector>
#include "Shader.hpp"
#include <unordered_map>

using namespace std;
using namespace glm;


static int ANIMATION_SLOWDOWN_FACTOR = 8;


class SelectionBox
{
public:
	SelectionBox(unordered_map<textureAttributes, vector<Texture>*> textures);
	~SelectionBox();
	vector<Vertex> makeSquareVertices(vec2 pointA, vec2 pointB);

	void draw(Shader& shader);
	void tick(float deltaTime);
	vec2 a;
	vec2 b;
	vec3 color;
	bool active;
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