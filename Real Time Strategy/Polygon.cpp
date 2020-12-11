#include "Polygon.hpp"

Polygon::Polygon(vector<int> pointsIn, bool includeMe)
{
	this->includeMe = includeMe;
	for (size_t i = 0; i < pointsIn.size()-2; i+=2)
	{
		this->points.push_back(vec2(pointsIn[i], pointsIn[i + 1]));
	}
	for (size_t i = 0; i < this->points.size(); i++)
	{
		this->edges.push_back(pair(this->points[i], this->points[(i + 1) % this->points.size()]));
	}
}

Polygon::~Polygon()
{
}