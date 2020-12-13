#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <string>

using namespace std;
using namespace glm;

struct Edge
{
    array<vec2, 2> points;

    void print(string prefix = "") const
    {
        printf("%s (%f,%f), (%f,%f)\n", prefix.c_str(), points[0].x, points[0].y, points[1].x, points[1].y);
    }


    template<typename T>
    bool operator==(const T& other)
    {
        try
        {
            const Edge* otherEdge = dynamic_cast<const Edge*>(&other);
            if (otherEdge == 0)
            {
                return false;
            }
            if (this->points[0] == otherEdge->points[0] && this->points[1] == otherEdge->points[1])
            {
                return true;
            }
            if (this->points[0] == otherEdge->points[1] && this->points[1] == otherEdge->points[0])
            {
                return true;
            }
            return false;
        }
        catch (exception e)
        {
            return false;
        }
        printf("END OF == OPERATOR BAD BAD BAD\n\n\n");
        return false;
    }
};

namespace std {
    template<>
    struct hash<Edge> {
        inline size_t operator()(const Edge& x) const
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
}



class Triangle
{
public:
    Triangle(array<vec2, 3> points);
    Triangle(vec2 a = vec2(0), vec2 b = vec2(0), vec2 c = vec2(0));
    ~Triangle();

    array<vec2, 3> points;
    array<Edge, 3> edges;
    vec2 circleCenter;
    vec2 geoCenter;
    vec2 mins;
    vec2 maxs;

    void print(string prefix = "") const;
    bool hasEdge(Edge e) const;

    template<typename T>
    bool operator==(const T& other) const;
    int contains(vec2 point) const;
    bool isAdjacent(const Triangle& other) const;
    bool shareAPoint(const Triangle& other) const;


private:
    vec2 makeCircleCenter();
};

float sign(vec2 a, vec2 b, vec2 p);
#endif;