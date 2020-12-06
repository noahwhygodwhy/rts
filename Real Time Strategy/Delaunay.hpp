#pragma once
#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <unordered_set>


using namespace std;
using namespace glm;



struct Triangle
{
    array<vec2, 3> points;

    template<typename T>
    bool operator==(const T& other)
    {
        if (typeid(other) == typeid(this))
        {
            Triangle otherTri = (Triangle)other;
            for (int i = 0; i < 3; i++)
            {
                if (this->points[i] != otherTri.points[i])
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};


namespace std {
    template<>
    struct hash<Triangle> {
        inline size_t operator()(const Triangle& x) const
        {
            hash<int> ih;

            size_t value = 0;

            for (vec2 p : x.points)
            {
                value = value ^ ih(p.x) & ih(p.y);
            }

            return value;
        }
    };

    template<>
    struct equal_to<Triangle> {
        inline bool operator()(const Triangle& a, const Triangle& b) const
        {
            for (int i = 0; i < 3; i++)
            {
                if (a.points[i] != b.points[i])
                {
                    return false;
                }
            }
            return true;
        }
    };

    bool operator==(const Triangle& a, const Triangle& b)
    {
        for (int i = 0; i < 3; i++)
        {
            if (a.points[i] != b.points[i])
            {
                return false;
            }
        }
        return true;
    }
    
    template<typename T> 
    bool operator==(Triangle& a, T& b)
    {
        if (type(b) == Triangle)
        {
            for (int i = 0; i < 3; i++)
            {
                if (a.points[i] != b.points[i])
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    //template<>
}




float sign(vec2 a, vec2 b, vec2 p)
{
    return (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
}

int inTriangle(const Triangle& t, vec2 point)
{
    float x = sign(point, t.points[0], t.points[1]);
    float y = sign(point, t.points[1], t.points[2]);
    float z = sign(point, t.points[2], t.points[0]);
    if (x == 0 || y == 0 || z == 0)
    {
        return -1;
    }
    bool neg = x < 0 || y < 0 || z < 0;
    bool pos = x > 0 || y > 0 || z > 0;

    return !(pos && neg); //not all of them, and not none of them
}

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



//TODO: This is wrong somehow, making bad triangles with misconfigured points
/*
###initialized, adding things
on point 0
starting with 1 triangles
it's in 1 triangles
this many triangles contain the point: 1:
splitting 1 triangles
s is -2716270.000000
adding a triangle
s is -856907.000000
adding a triangle
s is -66204.000000
adding a triangle
splitting into 3 triangles
splitting into 3 triangles
ending with 4 triangles
on point 1
starting with 4 triangles
it's in 1 triangles
this many triangles contain the point: 1:
splitting 1 triangles
s is -2112136.000000
adding a triangle
s is -922214.000000
adding a triangle
s is -605031.000000
adding a triangle
splitting into 3 triangles
splitting into 3 triangles
ending with 7 triangles
ERROR IN checkMiddleEdge() in Delanuay.hpp
triangle 1: (5.000000, 34.000000), (5.000000, 1873.000000), (5.000000, 34.000000)
triangle 2: (5.000000, 34.000000), (5.000000, 1873.000000), (41.000000, 467.000000)
unique points:
41.000000, 467.000000
shared points:
5.000000, 34.000000
5.000000, 1873.000000
5.000000, 34.000000
5.000000, 34.000000
5.000000, 1873.000000
*/
vector<Triangle> splitTriangle(vector<Triangle> tris, vec2 p)
{
    printf("splitting %i triangles\n", tris.size());
    vector<Triangle> newTriangles;

    for (const Triangle& t : tris)
    {
        for (int i = 0; i < 3; i++)
        {
            
            Triangle theNewOne = { t.points };
            theNewOne.points[i] = p;

            float s = sign(theNewOne.points[(i + 1) % 3], theNewOne.points[(i + 2) % 3], theNewOne.points[i]);
            printf("s is %f\n", s);
            if (s != 0)
            {
                printf("adding a triangle\n");
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

bool adjacent(const Triangle& a, const Triangle& b)
{
    int shared = 0;
    for(int i = 0; i < 3; i++)
    {
        shared += a.points[i] == b.points[i] ? 1 : 0;
    }
    return (shared == 2);
}

float distance(const vec2& p1, const vec2& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

void checkMiddleEdge(Triangle& a, Triangle& b)
{
    vector<vec2> points;
    points.insert(points.end(), a.points.begin(), a.points.end());
    points.insert(points.end(), b.points.begin(), b.points.end());
    vector<vec2> uniquePoints;
    vector<vec2> sharedPoints;
    for (vec2 v : points)
    {
        if (count(points.begin(), points.end(), v) == 1)
        {
            uniquePoints.push_back(v);
        }
        else
        {
            sharedPoints.push_back(v);
        }
    }
    if (uniquePoints.size() != 2 || sharedPoints.size() != 4)
    {

        printf("ERROR IN checkMiddleEdge() in Delanuay.hpp\n");
        printf("triangle 1: (%f, %f), (%f, %f), (%f, %f)\n", a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.points[2].x, a.points[2].y);
        printf("triangle 2: (%f, %f), (%f, %f), (%f, %f)\n", b.points[0].x, b.points[0].y, b.points[1].x, b.points[1].y, b.points[2].x, b.points[2].y);
        printf("unique points:\n");
        for (vec2 v : uniquePoints)
        {
            printf("%f, %f\n", v.x, v.y);
        }
        printf("shared points:\n");
        for (vec2 v : sharedPoints)
        {
            printf("%f, %f\n", v.x, v.y);
        }
        exit(-1);
    }
    float curDistance = distance(sharedPoints[0], sharedPoints[1]);
    float posDistance = distance(uniquePoints[0], uniquePoints[1]);
    //if it's illegal, flip the edge.
    if (posDistance < curDistance)
    {
        a.points = { {sharedPoints[0], sharedPoints[1], points[0]} };
        b.points = { {sharedPoints[0], sharedPoints[1], points[1]} };
    }
}

void fixIllegalTriangles(vector<Triangle>& tris)
{
    for (Triangle t : tris)
    {
        for (Triangle ta : tris)
        {
            if (adjacent(t, ta))
            {
                checkMiddleEdge(t, ta);   
            }
        }
    }
}
vector<Triangle> delaunay(vector<vec2> pointsIn)
{
    vector<Triangle> triangles;
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }
    //super triangle
    triangles.push_back({ mins-vec2(1), 
        vec2(mins.x-1, mins.y + ((maxs.y - mins.y) * 2)), 
        vec2(mins.x + ((maxs.x - mins.x) * 2), mins.y-1) });
    
    //srand(time(nullptr));
    int i = 0;
    for (vec2 point : pointsIn)
    {
        printf("on point %i\n", i++);
        printf("starting with %i triangles\n", triangles.size());
        vector<Triangle> containingTriangle = findContainer(triangles, point);
        printf("this many triangles contain the point: %i:\n", containingTriangle.size());

        vector<Triangle> newTriangles = splitTriangle(containingTriangle, point);
        printf("splitting into %i triangles\n", newTriangles.size());

        int i2 = 0;
        for (Triangle t : containingTriangle)
        {
            triangles.erase(remove(triangles.begin(), triangles.end(), t), triangles.end());
        }

        printf("splitting into %i triangles\n", newTriangles.size());
        triangles.insert(triangles.end(), newTriangles.begin(), newTriangles.end());
        printf("ending with %i triangles\n", triangles.size());
        fixIllegalTriangles(triangles);
        //Then go through and flip all illegal triangles
        //http://web.mit.edu/alexmv/Public/6.850-lectures/lecture09.pdf
    }
    printf("returning %i triangles\n", triangles.size());
    return triangles;
}

#endif