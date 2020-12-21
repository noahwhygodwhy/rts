#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <string>
//#include "NavMesh.hpp"

using namespace std;
using namespace glm;


bool counterClockwise(vec2 a, vec2 b, vec2 c);

struct Edge;

struct Edge
{
    array<vec2, 2> points;

    void print(string prefix = "") const
    {
        printf("%s (%f,%f), (%f,%f)\n", prefix.c_str(), points[0].x, points[0].y, points[1].x, points[1].y);
    }
    bool intersects(const Edge& other) const
    {
        vec2 a = this->points[0];
        vec2 b = this->points[1];
        vec2 c = other.points[0];
        vec2 d = other.points[1];

        if (a == c || a == d || b == c || b == d) //cause we're ok with tips touching
        {
            return false;
        }
        return counterClockwise(a, c, d) != counterClockwise(b, c, d) && counterClockwise(a, b, c) != counterClockwise(a, b, d);
    }
    float length() const
    {
        return distance(this->points[0], this->points[1]);
    }



    template<typename T>
    bool operator<(const T& other) const
    {
        return false;//lol how is an edge lesser than another edge
    }

    template<typename T>
    bool operator==(const T& other) const
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

    int contains(vec2 point) const;
    bool isAdjacent(const Triangle& other) const;
    bool shareAPoint(const Triangle& other) const;
    vec2 closestPoint(const vec2& p) const;
    Edge* getAdjacentEdge(const Triangle& other) const;

    /*size_t operator()(const Triangle& tri)
    {
        hash<float> fh;

        size_t value = 0;

        for (vec2 p : tri.points)
        {
            value = value ^ fh(p.x) & fh(p.y);
        }

        return value;
    }*/
    /*
    struct HashFunction{
        inline size_t operator()(const Triangle& x) const
        {
            hash<float> fh;

            size_t value = 0;

            for (vec2 p : x.points)
            {
                value = value ^ fh(p.x) & fh(p.y);
            }

            return value;
        }
    };*/


private:
    vec2 makeCircleCenter();
    vec2 centroid();
};

float sign(vec2 a, vec2 b, vec2 p);



namespace std {
    /*template<typename T>
    bool operator==(const Triangle& t, const T& other)
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
                if (t.points[i] != otherTri->points[i])
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
    template<typename T>
    bool operator==(const T& other, const Triangle& t)
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
                if (t.points[i] != otherTri->points[i])
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

    bool operator==(const Triangle& t, const Triangle& other)
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
                if (t.points[i] != otherTri->points[i])
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
    }*/


    template<>
    struct hash<Triangle> {
        size_t operator()(const Triangle& x) const
        {
            hash<float> fh;

            size_t value = 0;

            for (vec2 p : x.points)
            {
                value = value ^ fh(p.x) & fh(p.y);
            }

            return value;
        }
    };
}


#endif;