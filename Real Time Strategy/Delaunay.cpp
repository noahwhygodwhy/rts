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
    printf("add a point, %f,%f\n", point.x, point.y);
    vector<Triangle> badTriangles;

    for (const Triangle& t : triangles) //for each triangle in triangles
    {
        vec2 center = t.circleCenter;
        //t.print();
        //printf("center: %f,%f\n", t.circleCenter.x, t.circleCenter.y);
        float radius = distance(t.points[0], center);
        if (distance(t.circleCenter, point) <= radius)
        {
            badTriangles.push_back(t);
        }
    }
    //printf("results in %i bad triangles\n", badTriangles.size());
    printf("size of bad triangles: %i\n", badTriangles.size());

    for (const Triangle& t : triangles)
    {
        if (t.contains(point))
        {
            t.print("contains the point: ");
        }
    }


    vector<Edge> polygon;
    for (const Triangle& t : badTriangles)//for each triangle in bad triangles
    {
        t.print("bad tringle: ");
        for (int i = 0; i < t.points.size(); i++) //for each edge
        {
            bool shared = false;
            for (const Triangle& ot : badTriangles)
            {
                if (!(ot == t))
                {
                    if (ot.hasEdge({ vec2(t.points[i]), vec2(t.points[(i + 1) % 3]) }))
                    {
                        shared = true;
                        break;
                    }
                }
            }
            if (!shared)
            {
                polygon.push_back({ t.points[i], t.points[(i + 1) % 3] });
            }
        }
    }
    for (const Triangle& t : badTriangles)
    {
        triangles.erase(find(triangles.begin(), triangles.end(), t));
    }
    for (const Edge& e : polygon)
    {
        Edge ab = { e.points[0], e.points[1] };
        Edge bc = { e.points[1], point };
        Edge ac = { e.points[0], point };
        if (!(ab.slope() == ac.slope() || ab.slope() == bc.slope()))
        {
            triangles.push_back({ e.points[0], e.points[1], point });
        }
    }
}


vector<Triangle> delaunay(const vector<vec2>& pointsIn, vec2 bottomLeft, vec2 topRight, vector<Triangle> triangles )
{
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }
    if (!triangles.empty())
    {
        for (const Triangle& t : triangles)
        {
            for (vec2 point : t.points)
            {
                mins = glm::min(mins, point);
                maxs = glm::max(maxs, point);
            }
        }
    }
    Triangle t1;
    Triangle t2;

    if (bottomLeft == topRight)
    {
        t1 = { mins - vec2(1), vec2(mins.x - 1, maxs.y + 1), vec2(maxs.x + 1, mins.y - 1) };
        t2 = { maxs + vec2(1), vec2(mins.x - 1, maxs.y + 1), vec2(maxs.x + 1, mins.y - 1) };
    }
    else
    {
        t1 = { bottomLeft-vec2(1), vec2(bottomLeft.x-1, topRight.y+1), topRight+ vec2(1) };
        t2 = { bottomLeft-vec2(1), vec2(topRight.x+1, bottomLeft.y-1), topRight + vec2(1)};
    }
    t1.print("t1: ");
    t2.print("t2: ");

    triangles.push_back(t1);
    triangles.push_back(t2);




    
    for (vec2 point : pointsIn)
    {
        addAPoint(triangles, point);
    }
    
    auto triIter = triangles.begin();
    while (triIter != triangles.end())
    {
        if (triIter->shareAPoint(t1))
        {
            triangles.erase(triIter);
        }
        else if (triIter->shareAPoint(t2))
        {
            triangles.erase(triIter);
        }
        else
        {
            triIter++;
        }
    }

    
    return triangles;
}