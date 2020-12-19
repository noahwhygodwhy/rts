#include "Delaunay.hpp"
#include "Triangle.hpp"
#include "UsefulStructs.hpp"

using namespace std;
using namespace glm;


float distance(const vec2& p1, const vec2& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

void addAPoint(vector<Triangle>& triangles, vec2 point)
{

    vector<Triangle> badTriangles;

    for (const Triangle& t : triangles) //for each triangle in triangles
    {
        vec2 center = t.circleCenter;
        float radius = distance(t.points[0], center);
        if (distance(t.circleCenter, point) <= radius)
        {
            badTriangles.push_back(t);
        }
    }
    vector<Edge> polygon;
    for (const Triangle& t : badTriangles)//for each triangle in bad triangles
    {
        for (int i = 0; i < t.points.size(); i++) //for each edge
        {
            bool shared = false;
            for (const Triangle& ot : badTriangles)
            {
                if (!(ot == t))
                {
                    if (ot.hasEdge({ vec2(t.points[i]), vec2(t.points[(i + 1) % t.points.size()]) }))
                    {
                        shared = true;
                        break;
                    }
                }
            }
            if (!shared)
            {
                polygon.push_back({ t.points[i], t.points[(i + 1) % t.points.size()] });
            }
        }
    }
    for (const Triangle& t : badTriangles)
    {
        triangles.erase(find(triangles.begin(), triangles.end(), t));
    }
    for (const Edge& e : polygon)
    {
        triangles.push_back({ e.points[0], e.points[1], point });
    }
}


vector<Triangle> delaunay(const vector<vec2>& pointsIn, vec2 bottomLeft, vec2 topRight, const vector<Edge>& requiredEdges)
{
    vector<Triangle> triangles;
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }

    Triangle superTriangle = { mins - vec2(1),
        vec2(mins.x - 10, mins.y + 10 + ((maxs.y - mins.y) * 2)),
        vec2(mins.x + 10 + ((maxs.x - mins.x) * 2), mins.y - 10) };
    if (bottomLeft == topRight)
    {
        triangles.push_back(superTriangle);
    }
    else
    {
        Triangle t1 = { bottomLeft, vec2(bottomLeft.x, topRight.y), topRight };
        Triangle t2 = { bottomLeft, vec2(topRight.x, bottomLeft.y), topRight };
        triangles.push_back(t1);
        triangles.push_back(t2);
    }

    for (vec2 point : pointsIn)
    {
        addAPoint(triangles, point);
    }

    if (bottomLeft == topRight)
    {
        auto triIter = triangles.begin();
        while (triIter != triangles.end())
        {
            if (triIter->shareAPoint(superTriangle))
            {
                triangles.erase(triIter);
            }
            else
            {
                triIter++;
            }
        }
    }
    return triangles;
}