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
//#include "DirectionEnum.h"

using namespace std;
using namespace glm;


class Camera
{

	// Euler Angles
public:
	vec2 position;
	// Camera options
	float movementSpeed;
	float zoom;
	int id;

	Camera(vec2 initPos, float mvspd, float zooooooom);
	Camera();

	//void keyboardInput(Direction dir, float time);
	void move(vec2 dir, float deltaTime);
	mat4 getView();
	void zoomCall (int direction);

};

#endif