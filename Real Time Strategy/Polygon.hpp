#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;
class Polygon
{
public:
	Polygon(vector<int> points);
	~Polygon();
	vector<vec2> points;
	vector<pair<vec2, vec2>> edges;
	bool innerFill;
private:
};

#endif
