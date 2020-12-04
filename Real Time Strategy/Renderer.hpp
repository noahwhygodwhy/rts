#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.hpp"
#include "Shader.hpp"
#include <vector>
#include "Entity.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "SelectionBox.hpp"

class Renderer
{
public:
	Renderer(int x, int  y);
	~Renderer();
	bool initialize();
	void run();
	void addEntity(Entity* th);
	void processInput(GLFWwindow* window, float deltaTime);
	Camera cam;
	vector<Entity*> things;
	float screenX;
	float screenY;
	mat4 projMat;
	SelectionBox sb;
private:
	Shader shader;
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO;
	
};


vec2 calculateMousePos(GLFWwindow* window);

#endif

Texture makeTexture(string filepath);