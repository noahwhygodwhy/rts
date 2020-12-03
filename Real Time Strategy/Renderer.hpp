#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.hpp"
#include "Shader.hpp"
#include <vector>
#include "Entity.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>

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
private:
	Shader shader;
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO;
	
};




#endif