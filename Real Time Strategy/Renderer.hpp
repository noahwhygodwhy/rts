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
#include "Map.hpp"


static int NUM_CHANNELS = 4;

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
	float screenX;
	float screenY;
	mat4 projMat;
private:
	Shader shader;
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO;
	
};


vec2 calculateMousePos(GLFWwindow* window);

#endif

Texture makeTexture(string filepath);
unsigned char* imageToBytes(string filepath, int* x, int* y, int* nrChannels);
void saveImage(string filename, unsigned char* data, int width, int height, int nrChannels);