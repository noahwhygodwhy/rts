#include "Polygon.hpp"

Polygon::Polygon(vector<int> pointsIn)
{
	for (size_t i = 0; i < pointsIn.size()-2; i+=2)
	{
		this->points.push_back(vec2(pointsIn[i], pointsIn[i + 1]));
	}
	for (size_t i = 0; i < this->points.size(); i++)
	{
		this->edges.push_back(pair(this->points[i], this->points[(i + 1) % this->points.size()]));
	}
	this->innerFill = true;
	for (vec2 v : this->points)
	{
		if (v == vec2(0))
		{
			this->innerFill = false;
			break;
		}
	}
}

Polygon::~Polygon()
{
}