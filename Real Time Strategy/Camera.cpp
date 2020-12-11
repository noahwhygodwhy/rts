#pragma once

#include <iostream>
//#include "DirectionEnum.h"
#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

static int asdfasdf = 0;

const float MINZOOM = 0.1f;
const float MAXZOOM = 5.0f;


Camera::Camera(vec2 initPos, float mvspd, float zooooooom)
{
	this->position = initPos;
	movementSpeed = mvspd;
	zoom = zooooooom;
	this->id = asdfasdf++;
}
Camera::Camera()
{
	this->position = vec2(0);
	movementSpeed = 1;
	zoom = 1;
	this->id = asdfasdf++;
}

void Camera::zoomCall(int direction)
{
	this->zoom += ((float)direction) * 0.1f;
	this->zoom = std::max(std::min(this->zoom, MAXZOOM), MINZOOM);
}

mat4 Camera::getView()
{
	mat4 toReturn = glm::lookAt(vec3(this->position.x, this->position.y, 0), vec3(this->position.x, this->position.y, -1), vec3(0, 1, 0));

	toReturn = glm::scale(toReturn, vec3(zoom, zoom, 1));

	return toReturn;
}

void Camera::move(vec2 dir, float deltaTime)
{
	this->position += vec2(1, -1) * dir * deltaTime * this->movementSpeed;
}
