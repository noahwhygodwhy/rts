#include "Delaunay.hpp"
#include "Triangle.hpp"

using namespace std;
using namespace glm;


vector<Triangle> findContainer(const vector<Triangle>& triangles, vec2 point)
{
    vector<Triangle> toReturn;
    for (const Triangle& t : triangles)
    {
        int result = inTriangle(t, point);
        printf("it's in %i triangles\n", result);
        if (result == 1)
        {
            toReturn.push_back(t);
            return toReturn;
        }
        else if (result == -1)
        {
            toReturn.push_back(t);
            if (toReturn.size() == 2)
            {
                return toReturn;
            }
        }
    }
    return toReturn; //just in case
}


vector<Triangle> splitTriangle(vector<Triangle> tris, vec2 p)
{
    printf("splitting %i triangles with point %f,%f\n", tris.size(), p.x, p.y);
    vector<Triangle> newTriangles;



    for (const Triangle& t : tris)
    {

        t.print("splitting");

        for (int i = 0; i < 3; i++)
        {

            Triangle theNewOne = { t.points };
            theNewOne.points[i] = p;

            float s = sign(theNewOne.points[(i + 1) % 3], theNewOne.points[(i + 2) % 3], theNewOne.points[i]);
            printf("s is %f\n", s);
            if (s != 0)
            {
                theNewOne.print("adding a triangle");
                newTriangles.push_back(theNewOne);
            }
            else
            {
                printf("not adding a triangle\n");
            }
        }
    }
    return newTriangles;
}


float distance(const vec2& p1, const vec2& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt((dx * dx) + (dy * dy));
}


void fixIllegalTriangles(vector<Triangle>& tris, const vector<vec2>& usedPoints)
{

    for (vec2 p : usedPoints)
    {
        auto triIter = tris.begin();
        while (triIter != tris.begin())
        {
            vec2 center = getCenter(*triIter);
            float radius = distance(center, triIter->points[0]);
            if (distance(p, center) < radius)
            {
                tris.erase(triIter);

                for (int i = 0; i < triIter->points.size(); i++)
                {
                    tris.push_back({ p, triIter->points[i], triIter->points[(i + 1) % triIter->points.size()] });
                }
            }
            else
            {
                triIter++;
            }
        }
    }



    /*
        for each triangle
            for each added point


            

    */


    /*vec2 center = getCenter(tri);
    for (vec2 p : points)
    {
        if (distance(p, center) < distance(tri.points[0], center)) //if it's in the circle
        {

        }
    }*/
    /*

    //TODO: this part but with caffeine 
    for (Triangle& t : tris)
    {
        if (!(t == tri))
        {
            if (adjacent(tri, t))
            {
                vec2 center = getCenter(tri);
                float radius = distance(center, tri.points[0]);

                vec2 tnotSharedPoint(0);
                vec2 sharedPoint1(0);
                vec2 sharedPoint2(0);
                vec2 trinotSharedPoint(0);
                for (vec2 x : t.points)
                {
                    if (count(tri.points.begin(), tri.points.end(), x) == 0)
                    {
                        tnotSharedPoint = x;
                        break;
                    }
                }
                for (vec2 x : tri.points)
                {
                    if (count(t.points.begin(), t.points.end(), x) == 0)
                    {
                        trinotSharedPoint = x;
                        break;
                    }
                }
                if (distance(center, tnotSharedPoint) < radius)
                {
                    Triangle a = { trinotSharedPoint, tnotSharedPoint, sharedPoint1 };
                    Triangle b = { trinotSharedPoint, tnotSharedPoint, sharedPoint2 };
                    t.points = a.points;
                    tri.points = b.points;
                    fixIllegalTriangles(tri, tris, points);
                    fixIllegalTriangles(t, tris, points);
                }
            }
        }
    }*/
}



void addAPoint(vector<Triangle>& triangles, vec2 point)
{

    vector<Triangle> badTriangles;

    for (const Triangle& t : triangles) //for each triangle in triangles
    {
        vec2 center = getCenter(t);
        float radius = distance(t.points[0], center);
        if (distance(center, point) <= radius)
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


vector<Triangle> delaunay(const vector<vec2>& pointsIn, vec2 bottomLeft, vec2 topRight)
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
            if (shareAPoint(superTriangle, *triIter))
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