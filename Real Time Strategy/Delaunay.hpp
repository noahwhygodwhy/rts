#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include <array>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

vector<array<vec2, 3>> delaunay(vector<vec2> pointsIn)
{
    vector<array<vec2, 3>> triangles;
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }
    //super triangle
    triangles.push_back({ mins, vec2(mins.x, mins.y + ((maxs.y - mins.y) * 2)), vec2(mins.x + ((maxs.x - mins.x) * 2), mins.y) );

}

#endif