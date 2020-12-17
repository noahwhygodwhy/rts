#ifndef NAV_MESH_H
#define NAV_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include "TriangleTree.hpp"
#include "Triangle.hpp"
#include <unordered_map>
#include <unordered_set>
#include "Shader.hpp"
#include "UsefulStructs.hpp"

using namespace std;
using namespace glm;


struct aStarNode
{
    float hcost;
    float gcost;
    float fcost;
    Triangle t;
    template <typename T>
    bool operator==(const T& other)
    {
        const aStarNode* b = dynamic_cast<const aStarNode*>(&other);
        if (b == 0)
        {
            return false;
        }
        return b->t == this->t;
    }
    bool operator<(const aStarNode& other) const
    {
        return this->fcost > other.fcost; //intentionally backwards
    }
    bool operator>(const aStarNode& other) const 
    {
        return this->fcost < other.fcost;
    }
};


namespace std
{
    template <>
    struct hash<vec2>
    {
        size_t operator()(const vec2 k) const
        {
            hash<float> fh;
            return fh(k.x) ^ (fh(k.y)<<1);
        }
    };

}
namespace std
{
    /*template <typename T>
    bool operator<(T& that, T& other)
    {
        try
        {
            vec2* a = dynamic_cast<vec2*>(&that);
            if (a == 0)
            {
                return true;
            }
            vec2* b = dynamic_cast<vec2*>(&other);
            if (b == 0)
            {
                return true;
            }
            return a < b;
        }
        catch (exception e)
        {
            return true;
        }
        printf("END OF < OPERATOR BAD BAD BAD\n\n\n");
        return false;
    }*/
}

class NavMesh
{
public:
	NavMesh();
    NavMesh(vector<Triangle> tris, unordered_set<Edge> fedges, int width = 0, int height = 0);
	~NavMesh();
	vector<vec2> getPath(vec2 start, vec2 end);
    void draw(const Shader& shader);
    
private:
    void setupBuffers();
    unsigned int VBO, VAO;
	vector<Triangle> tris;
	TriangleTree triTree;
	unordered_map<Triangle, vector<Triangle*>> adjacencySet;
    unordered_set<Edge> fedges;
    vector<Vertex> vertices;
};


#endif