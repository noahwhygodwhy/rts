#pragma once
#include "Controller.hpp"

//Basic Controller
Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::tick(float deltatime)
{
	printf("basic tick\n");
}

//Static controller, for things that don't move
StaticController::StaticController() : Controller()
{
}

StaticController::~StaticController()
{
}

void StaticController::tick(float deltatime)
{
	printf("static tick\n");
}