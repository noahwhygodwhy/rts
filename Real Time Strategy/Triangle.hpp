#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H


#include <vector>
#include <array>
#include <glm/glm.hpp>


struct Triangle
{
    array<vec2, 3> points;

    void print(string prefix = "") const
    {
        printf("%s (%f,%f), (%f,%f), (%f,%f)\n", prefix.c_str(), points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
    }

    bool hasEdge(const Edge& e) const
    {
        int shared = 0;
        for (vec2 p : points)
        {
            for (vec2 v : e.points)
            {
                if (p == v)
                {
                    shared += 1;
                }
            }
        }
        if (shared > 2)
        {
            printf("TRIANGLE HAS EDGE BROKE\n");
            exit(0);
        }
        return shared == 2;
    }
    template<typename T>
    bool operator==(const T& other) const
    {
        try
        {
            const Triangle* otherTri = dynamic_cast<const Triangle*>(&other);
            if (otherTri == 0)
            {
                return false;
            }
            for (int i = 0; i < 3; i++)
            {
                if (this->points[i] != otherTri->points[i])
                {
                    return false;
                }
            }
            return true;
        }
        catch (exception e)
        {
            return false;
        }
        printf("END OF == OPERATOR BAD BAD BAD\n\n\n");
        return false;
    }
};



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


bool adjacent(const Triangle& a, const Triangle& b)
{
    int shared = 0;
    for (int i = 0; i < 3; i++)
    {
        shared += a.points[i] == b.points[i] ? 1 : 0;
    }
    return (shared == 2);
}

//TODO: what if the 3 points are inline
vec2 getCenter(const Triangle& t)
{
    float middleX = NAN;
    float middleY = NAN;

    vec2 midAB = (t.points[0] + t.points[1]) / 2.0f;
    vec2 midBC = (t.points[1] + t.points[2]) / 2.0f;
    float slopeAB = ((t.points[0].y - t.points[1].y) / (t.points[0].x - t.points[1].x));
    float tangentAB;
    //if the slope is infinity, the tangent is 0, else, calculate the tangent
    if (slopeAB == INFINITY || slopeAB == -INFINITY)
    {
        tangentAB = 0;
    }
    else
    {
        tangentAB = -1 / slopeAB;
    }
    //if the tangent is infinity, the middle X is the middle of points
    //if it's 0, middle Y is the middle of points
    //else calculate the y intercept
    float yIntTanAB = NAN;
    if (tangentAB == INFINITY || tangentAB == -INFINITY)
    {
        middleX = (t.points[0].x + t.points[1].x) / 2;
    }
    else if (tangentAB == 0)
    {
        middleY = (t.points[0].y + t.points[1].y) / 2.0f;
    }
    else
    {
        yIntTanAB = midAB.y - (tangentAB * midAB.x);
    }

    float slopeBC = ((t.points[1].y - t.points[2].y) / (t.points[1].x - t.points[2].x));
    float tangentBC;
    //if the slope is infinity, the tangent is 0, else, calculate the tangent
    if (slopeBC == INFINITY || slopeBC == -INFINITY)
    {
        tangentBC = 0;
    }
    else
    {
        tangentBC = -1 / slopeBC;
    }

    //if the tangent is infinity, the middle X is the middle of points
    //if it's 0, middle Y is the middle of points
    //else calculate the y intercept
    float yIntTanBC = NAN;
    if (tangentBC == INFINITY || tangentBC == -INFINITY)
    {

        middleX = (t.points[1].x + t.points[2].x) / 2.0f;
    }
    else if (tangentBC == 0)
    {
        middleY = (t.points[1].y + t.points[2].y) / 2.0f;
    }
    else
    {
        yIntTanBC = midBC.y - (tangentBC * midBC.x);
    }

    if (isnan(middleX) && isnan(middleY))
    {
        middleX = (yIntTanBC - yIntTanAB) / (tangentAB - tangentBC);
        middleY = (tangentAB * middleX) + yIntTanAB;
    }
    else if (isnan(middleX))
    {
        if (isnan(yIntTanAB) || isnan(tangentAB))
        {
            middleX = (middleY - yIntTanBC) / tangentBC;
        }
        else
        {
            middleX = (middleY - yIntTanAB) / tangentAB;
        }
    }
    else if (isnan(middleY))
    {
        if (isnan(yIntTanAB) || isnan(tangentAB))
        {
            middleY = (tangentBC * middleX) + yIntTanBC;
        }
        else
        {
            middleY = (tangentAB * middleX) + yIntTanAB;
        }
    }

    return vec2(middleX, middleY);
}


#endif;