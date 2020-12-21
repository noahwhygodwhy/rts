#pragma once
#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <unordered_set>
#include "Triangle.hpp"

using namespace std;
using namespace glm;

float distance(const vec2& p1, const vec2& p2);



void addAPoint(vector<Triangle>& triangles, vec2 point);


vector<Triangle> delaunay(const vector<vec2>& pointsIn, vec2 bottomLeft = vec2(0), vec2 topRight = vec2(0));

#endif