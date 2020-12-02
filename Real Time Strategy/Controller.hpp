#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "glm/glm.hpp"

using namespace std;
using namespace glm;

class Controller
{
public:
	void tick(float deltaTime);
private:
	
};

class StaticController : public Controller
{
public:
	StaticController();
	~StaticController();
	virtual void tick(float deltaTime);
private:

};


#endif
