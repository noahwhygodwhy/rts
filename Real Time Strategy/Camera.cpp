#pragma once

#include <iostream>
#include "DirectionEnum.h"
#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

//static int asdfasdf = 0;

Camera::Camera(vec3 initPos, vec3 worldUp, float yaw, float pitch, float mvspd, float mouseSens, float zooooooom)
{
	this->transform = mat4(1.0f);
	this->transform = glm::translate(this->transform, initPos);
	this->worldUp = worldUp;
	//this->transform[1] = vec4(initUp, this->transform[1][3]);

	yaw = yaw;
	pitch = pitch;
	movementSpeed = mvspd;
	mouseSensitivity = mouseSens;
	zoom = zooooooom;
	updateVectors();
}
Camera::Camera()
{
	this->transform = mat4(1.0f);
	yaw = 0;
	pitch = 0;
	movementSpeed = 1;
	mouseSensitivity = 1;
	zoom = 1;
	//updateVectors();
}


mat4 Camera::getView(mat4 parentTransform)
{
	mat4 t = parentTransform * this->transform;
	mat4 la =  glm::lookAt(vec3(t[3]), vec3(t[3]) - vec3(t[2]), vec3(t[1]));
	return la;
}

void Camera::mouseInput(float xpos, float ypos, GLboolean cp)
{

}

void Camera::updateVectors()
{
	vec3 f;
	f.x = cos(radians(yaw)) * cos(radians(pitch));
	f.y = sin(radians(pitch));
	f.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(f);
	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
	this->transform[0] = vec4(right, transform[0][3]);
	this->transform[1] = vec4(up, transform[1][3]);
	this->transform[2] = vec4(-front, transform[2][3]);
}




/*void Camera::keyboardInput(Direction dir, float time)
{
	float vel = time * movementSpeed;
	switch (dir)
	{
	case Direction::FORWARD:
		position += front * vel;
		break;
	case Direction::BACKWARD:
		position += -front * vel;
		break;
	case Direction::RIGHT:
		position += right * vel;
		break;
	case Direction::LEFT:
		position += -right * vel;
		break;
	case Direction::UP:
		position += up * vel;
		break;
	case Direction::DOWN:
		position += -up * vel;
		break;
	case Direction::YAW_LEFT:
		yaw += -vel * 3;
		break;
	case Direction::YAW_RIGHT:
		yaw += vel * 3;
		break;
	default:
		//this block intentionally left blank
		break;
	}
	////printf("new position: ");
	//printVecThree(position);
}*/