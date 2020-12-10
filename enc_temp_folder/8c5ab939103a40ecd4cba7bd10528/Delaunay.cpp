#include "Delaunay.hpp"


using namespace std;
using namespace glm;

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

bool adjacent(const Triangle& a, const Triangle& b)
{
    int shared = 0;
    for (int i = 0; i < 3; i++)
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
        a.print("triangle 1 ");
        b.print("triangle 2");
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


//TODO: what if the 3 points are inline
vec2 getCenter(const Triangle& t)
{
    bool debug = false;

    float middleX = NAN;
    float middleY = NAN;

    vec2 midAB = (t.points[0] + t.points[1]) / 2.0f;
    vec2 midBC = (t.points[1] + t.points[2]) / 2.0f;
    if (debug)
    {
        printf("A: %f,%f\n", t.points[0].x, t.points[0].y);
        printf("B: %f,%f\n", t.points[1].x, t.points[1].y);
        printf("C: %f,%f\n", t.points[2].x, t.points[2].y);
    }
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
    if (debug)
    {
        printf("slopeAB: %f\n", slopeAB);
        printf("tangentAB: %f\n", tangentAB);
    }
    //if the tangent is infinity, the middle X is the middle of points
    //if it's 0, middle Y is the middle of points
    //else calculate the y intercept
    float yIntTanAB = NAN;
    if (tangentAB == INFINITY || tangentAB == -INFINITY)
    {
        middleX = (t.points[0].x+t.points[1].x)/2;
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
    if (debug)
    {
        printf("slopeBC: %f\n", slopeBC);
        printf("tangentBC: %f\n", tangentBC);
    }

    //TODO: run it againand fix the missing values

    //if the tangent is infinity, the middle X is the middle of points
    //if it's 0, middle Y is the middle of points
    //else calculate the y intercept
    float yIntTanBC = NAN;
    if (tangentBC == INFINITY || tangentBC == -INFINITY)
    {

        middleX = (t.points[1].x + t.points[2].x)/2.0f;
    }
    else if (tangentBC == 0)
    {
        middleY = (t.points[1].y + t.points[2].y) / 2.0f;
    }
    else
    {
        yIntTanBC = midBC.y - (tangentBC * midBC.x);
    }

    if (debug)
    {
        printf("middleX: %f\n", middleX);
        printf("middleY: %f\n", middleY);
        printf("tangentAB: %f\n", tangentAB);
        printf("tangentBC: %f\n", tangentBC);
        printf("yIntTanAB: %f\n", yIntTanAB);
        printf("yIntTanBC: %f\n", yIntTanBC);
    }
    if (isnan(middleX) && isnan(middleY))
    {
        if (debug)
            printf("both NAN\n");
        middleX = (yIntTanBC - yIntTanAB) / (tangentAB - tangentBC);
        middleY = (tangentAB * middleX) + yIntTanAB;
    }
    else if (isnan(middleX))
    { 
        if(debug)
            printf("x NAN\n");
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
        if(debug)
            printf("y NAN\n");
        //middleY = (tangentAB * middleX) + yIntTanAB;
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


bool shareAPoint(Triangle& t, Triangle& superTriangle)
{
    for (vec2 a : t.points)
    {
        for (vec2 b : superTriangle.points)
        {
            if (a == b)
            {
                return true;
            }
        }
    }
    return false;
}

vector<Triangle> delaunayTwo(vector<vec2> pointsIn)
{
    vector<Triangle> triangles;
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }
    printf("mins: %f, %f, maxes: %f, %f\n", mins.x, mins.y, maxs.x, maxs.y);
    //super triangle
    Triangle superTriangle = { mins - vec2(1),
        vec2(mins.x - 1, mins.y + 10 + ((maxs.y - mins.y) * 2)),
        vec2(mins.x + 10 + ((maxs.x - mins.x) * 2), mins.y - 1) };

    triangles.push_back(superTriangle);
    for (vec2 point : superTriangle.points)
    {
        pointsIn.push_back(point);
    }

    //srand(time(nullptr));
    int i = 0;
    vector<vec2> usedPoints;
    for (vec2 point : pointsIn)
    {
        usedPoints.push_back(point);
        printf("on point %i\n", i++);
        printf("starting with %llu triangles\n", triangles.size());
        vector<Triangle> containingTriangle = findContainer(triangles, point);
        printf("this many triangles contain the point: %llu:\n", containingTriangle.size());

        vector<Triangle> newTriangles = splitTriangle(containingTriangle, point);
        printf("splitting into %llu triangles\n", newTriangles.size());


        printf("erasing %llu triangles\n", containingTriangle.size());

        for (Triangle t : containingTriangle)
        {
            auto toErase = find(triangles.begin(), triangles.end(), t);
            triangles.erase(toErase);
            //std::remove(triangles.begin(), triangles.end(), t);
        }

        printf("inserting %llu triangles\n", newTriangles.size());

        triangles.insert(triangles.end(), newTriangles.begin(), newTriangles.end());
        fixIllegalTriangles(triangles, usedPoints);
        
        //Then go through and flip all illegal triangles
        //http://web.mit.edu/alexmv/Public/6.850-lectures/lecture09.pdf
    }
    printf("returning %llu triangles\n", triangles.size());



    auto t = triangles.begin();
    while (t != triangles.end())
    {
        if (shareAPoint(*t, superTriangle))
        {
            triangles.erase(t);
        }
        else
        {
            t++;
        }
    }
    return triangles;
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
                    if (ot.hasEdge({ t.points[i], t.points[(i + 1) % t.points.size()] }))
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


vector<Triangle> delaunay(const vector<vec2>& pointsIn)
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

    triangles.push_back(superTriangle);
    
    for (vec2 point : pointsIn)
    {
        addAPoint(triangles, point);
    }
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
    return triangles;
}

vector<Triangle> delaunayOne(vector<vec2> pointsIn)
{
    vector<Triangle> triangles;
    vec2 mins = vec2(INT64_MAX);
    vec2 maxs = vec2(INT64_MIN);
    for (vec2 point : pointsIn)
    {
        mins = glm::min(mins, point);
        maxs = glm::max(maxs, point);
    }
    printf("mins: %f, %f, maxes: %f, %f\n", mins.x, mins.y, maxs.x, maxs.y);
    //super triangle
    Triangle superTriangle = { mins - vec2(1),
        vec2(mins.x - 1, mins.y + 10 + ((maxs.y - mins.y) * 2)),
        vec2(mins.x + 10 + ((maxs.x - mins.x) * 2), mins.y - 1) };

    triangles.push_back(superTriangle);
    printf("Super triangle: \n");
    for (vec2 point : superTriangle.points)
    {
        printf("%f,%f\n", point.x, point.y);
        pointsIn.push_back(point);
    }

    int c = 0;
    for (vec2 point : pointsIn)
    {
        printf("point: %f,%f\n", point.x, point.y);
        int c1 = 0;
        printf("on point %i\n", c++);
        vector<Edge> edgeBuffer;
        auto tri = triangles.begin();
        while(tri != triangles.end())
        {
            printf("on triangle %i\n", c1++);
            tri->print("the triangle: ");
            vec2 center = getCenter(*tri);
            printf("center: %f,%f\n", center.x, center.y);
            float radius = distance(tri->points[0], center);
            printf("radius: %f\n", radius);
            if (distance(center, point) < radius)
            {
                printf("point is in the triangle\n");
                for (int i = 0; i < tri->points.size(); i++)
                {
                    edgeBuffer.push_back({ tri->points[i], tri->points[(i + 1) % tri->points.size()] });
                }
                triangles.erase(tri);

            }
            else
            {
                tri++;
                printf("point is not in the triangle\n");
            }
            printf("---------------------\n");
        }
        printf("generated %lu edges\n", edgeBuffer.size());


        auto p = edgeBuffer.begin();
        while(p != edgeBuffer.end())
        {
            int howMany = count(edgeBuffer.begin(), edgeBuffer.end(), *p);
            if (howMany > 1)
            {
                
                for (int i = 0; i < howMany; i++)
                {
                    auto toErase = find(edgeBuffer.begin(), edgeBuffer.end(), *p);
                    edgeBuffer.erase(toErase);
                }
            }
            else
            {
                p++;
            }
        }

        for (Edge p : edgeBuffer)
        {
            triangles.push_back({ point, p.points[0], p.points[1] });
        }
        printf("now up to %lu triangles\n", triangles.size());
        printf("=================================\n\n");
        
    }

    /*printf("out of main process\n");
    auto v = pointsIn.begin();
    while (v != pointsIn.end())
    {
        for (vec2 no : superTriangle.points)
        {
            if (*v == no)
            {
                pointsIn.erase(v);
            }
        }
    }*/

    for (vec2 no : superTriangle.points)
    {
        auto v = pointsIn.begin();
        while (v != pointsIn.end())
        {
            if (*v == no)
            {
                pointsIn.erase(v);
            }
            else
            {
                v++;
            }
        }
    }


    auto t = triangles.begin();
    while (t != triangles.end())
    {
        if (shareAPoint(*t, superTriangle))
        {
            triangles.erase(t);
        }
        else
        {
            t++;
        }
    }

    printf("returning %llu triangles\n", triangles.size());

    return triangles;
}