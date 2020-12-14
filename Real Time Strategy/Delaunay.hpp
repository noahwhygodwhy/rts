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

//vector<Triangle> findContainer(const vector<Triangle>& triangles, vec2 point);

//vector<Triangle> splitTriangle(vector<Triangle> tris, vec2 p);

float distance(const vec2& p1, const vec2& p2);

//void checkMiddleEdge(Triangle& a, Triangle& b);

//void fixIllegalTriangles(vector<Triangle>& tris, const vector<vec2>& points);

vector<Triangle> delaunay(const vector<vec2>& pointsIn, vec2 bottomLeft = vec2(0), vec2 topRight = vec2(0));

#endif