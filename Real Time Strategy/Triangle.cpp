#include "Triangle.hpp"


using namespace std;
using namespace glm;

//TODO: what if the 3 points are inline
vec2 Triangle::makeCircleCenter()
{
    Triangle t = *this;
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

//returns the centroid of a triangle (the center point)
vec2 Triangle::centroid()
{
    Triangle t = *this;
    return vec2(((t.points[0].x + t.points[1].x + t.points[2].x) / 3), ((t.points[0].y + t.points[1].y + t.points[2].y) / 3));
}

Triangle::Triangle(array<vec2, 3> points) : Triangle(points[0], points[1], points[2])
{
}

Triangle::Triangle(vec2 a, vec2 b, vec2 c)
{
    //make sure counter clockwise treatment isn't fucking up anything
    this->points = { a, b, c };
    edges[0] = { a, b };
    edges[1] = { b, c };
    edges[2] = { c, a };
    this->circleCenter = makeCircleCenter();
    this->geoCenter = centroid();
    this->mins = glm::min(glm::min(points[0], points[1]), points[2]);
    this->maxs = glm::max(glm::max(points[0], points[1]), points[2]);

}

Triangle::~Triangle()
{

}


void Triangle::print(string prefix) const
{
    printf("%s (%f,%f), (%f,%f), (%f,%f)\n", prefix.c_str(), points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
}
bool Triangle::hasEdge(Edge e) const
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



vec2 Triangle::closestPoint(const vec2& p) const
{
    if (this->contains(p))
    {
        return vec2(p);
    }

    float s1 = sign(this->points[0], this->points[1], p);
    float s2 = sign(this->points[1], this->points[2], p);
    float s3 = sign(this->points[2], this->points[0], p);


    vec2 A;
    vec2 B;

    float theMin = std::min(s1, std::min(s2, s3));

    if (theMin == s1)
    {
        A = this->points[0];
        B = this->points[1];
    }
    else if (theMin == s2)
    {
        A = this->points[1];
        B = this->points[2];
    }
    else
    {
        A = this->points[2];
        B = this->points[0];
    }

    vec2 v = (B - A);
    float l = glm::dot((p - A), v) / glm::dot(v, v);
    vec2 s = vec2(0);
    if (l <= 0)
    {
        s = A;
    }
    else if (l >=1)
    {
        s = B;
    }
    else
    {
        s = A + (l * v);
    }
    return s;
}

//returns the distance of point p from the line containing points a and b
float sign(vec2 a, vec2 b, vec2 p)
{
    return (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
}

int Triangle::contains(vec2 point) const
{
    float x = sign(point, this->points[0], this->points[1]);
    float y = sign(point, this->points[1], this->points[2]);
    float z = sign(point, this->points[2], this->points[0]);

    if (x == 0 || y == 0 || z == 0)
    {
        return -1;
    }
    bool neg = x < 0 || y < 0 || z < 0;
    bool pos = x > 0 || y > 0 || z > 0;

    return !(pos && neg); //not all of them, and not none of them
}




Edge* Triangle::getAdjacentEdge(const Triangle& other) const
{
    for (const Edge& e1 : this->edges)
    {
        for (const Edge& e2 : other.edges)
        {
            if (e1 == e2)
            {
                return new Edge(e1);
            }
        }
    }
    return 0; //just incase they have no adjacent edge
}



bool Triangle::isAdjacent(const Triangle& other) const
{
    int shared = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            shared += this->points[i] == other.points[j] ? 1 : 0;
        }
    }
    if (shared > 3)
    {
        printf("SHARED > 3\n");
        exit(0);
    }
    return (shared == 2);
}

bool Triangle::shareAPoint(const Triangle& other) const
{
    for (vec2 a : this->points)
    {
        for (vec2 b : other.points)
        {
            if (a == b)
            {
                return true;
            }
        }
    }
    return false;
}