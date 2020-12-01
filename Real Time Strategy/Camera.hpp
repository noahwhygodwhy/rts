#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include "glad.h"
//#include "Asset.hpp"
//#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "DirectionEnum.h"

using namespace std;
using namespace glm;


class Camera
{

	// Euler Angles
public:
	mat4 transform;

	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;
	vec3 worldUp;
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	float lastX = 69.696969;
	float lastY = 69.696969;
	//int id;

	Camera(vec3 initPos, vec3 initUp, float y, float p, float mvspd, float mouseSens, float zooooooom);
	Camera();

	void changeWindowSize(fvec2 windowSize)
	{
		printf("chaing window size to %f,%f\n", windowSize.x, windowSize.y);
		lastX = ((float)windowSize.x) / 2;
		lastY = ((float)windowSize.y) / 2;

	}
	//void keyboardInput(Direction dir, float time);
	void mouseInput(float x, float y, GLboolean cp);
	void updateVectors();
	mat4 getView(mat4 parentTransform);

};

#endif