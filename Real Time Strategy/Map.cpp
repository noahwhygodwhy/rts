
#include "Renderer.hpp"
#include "Map.hpp"
#include "tinyxml2.h"
#include "Polygon.hpp"
#include <array>
#include "TriangleTree.hpp"
#include <unordered_set>
using namespace glm;
using namespace std;



vector<string> splitOnSpaces(string s)
{
    stringstream toSplit(s);
    string segment;
    vector<string> toReturn;
    while (getline(toSplit, segment, ' '))
    {
        toReturn.push_back(segment);
    }
    return toReturn;
}


Texture makeNavMesh(string filePath)
{


    int width, height, nrChannels;
    unsigned char* originalImage = imageToBytes(filePath.c_str(), &width, &height, &nrChannels);
    printf("original image channels: %i\n", NUM_CHANNELS);

    printf("original image: %p\n", originalImage);

    printf("making sobel\n");
    //unsigned char* sobelImage = makeSobalImage(originalImage, width, height, NUM_CHANNELS);
    //saveImage("SOBEL_IMAGE8325.png", sobelImage, width, height, NUM_CHANNELS);
    //saveImage("SOBEL_IMAGE8325.png", originalImage, width, height, nrChannels);
    printf("done sobel\n");

    Texture t;

    t.type = "diffuse";
    t.path = filePath;
    
    unsigned int texture;
    //printf("%p\n", &texture);
    glGenTextures(1, &texture);
    //printf("%i\n", texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    printf("about to use sobel\n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, originalImage);
    printf("used sobel\n");
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    t.id = texture;
    t.dims = vec2(width, height);
    t.channels = NUM_CHANNELS;
    
    return t;
}

bool counterClockwise(Triangle t)
{
    return (t.points[1].x - t.points[0].x) * (t.points[2].y - t.points[0].y) > (t.points[2].x - t.points[0].x) * (t.points[1].y - t.points[0].y);
}

vector<Triangle> generateNavMeshVerts(string inFilePath, string outFilePath, vec2 dims, unordered_set<Edge>* fedge)
{

    printf("reading file %s\n", inFilePath.c_str());
    vector<Polygon> shapes;
    tinyxml2::XMLDocument theFile;
    theFile.LoadFile(inFilePath.c_str());
    printf("%p\n", &theFile);
    tinyxml2::XMLElement* g = theFile.FirstChildElement("svg")->FirstChildElement("g");
    bool first = true;
    for (tinyxml2::XMLElement* ele = g->FirstChildElement(); ele; ele = ele->NextSiblingElement())
    {
        vector<string> pointStrings = splitOnSpaces(ele->FindAttribute("points")->Value());
        vector<int> pointInts;
        for (string s : pointStrings)
        {
            pointInts.push_back(stoi(s));
        }
        shapes.push_back(Polygon(pointInts, ele->FindAttribute("style")->Value()=="fill: none"));
    }
    vector<vec2> allPoints;
    for (Polygon x : shapes)
    {
        allPoints.insert(allPoints.end(), x.points.begin(), x.points.end());
    }
    printf("given dims: %f,%f\n", dims.x, dims.y);
    /*dims = vec2(1000);
    vector<vec2> allPoints;
    srand(42);
    for (int i = 0; i < 66; i++)
    {
        int x = rand() % 1000;
        int y = rand() % 1000;
        allPoints.push_back(vec2(x, y));
        printf("point %i: %i,%i\n",i, x, y);
    }
    /*allPoints.push_back(vec2(175, 400));
    allPoints.push_back(vec2(83, 879));
    allPoints.push_back(vec2(16, 644));
    allPoints.push_back(vec2(175, 400));
    allPoints.push_back(vec2(15, 55));*/
    vector<Triangle> triangles = delaunay(allPoints, vec2(0), dims); //not guarenteed to be CCW

    printf("size of triangles: %lu: \n", triangles.size());

    for (Polygon p : shapes)
    {
        if (!p.includeMe)
        {
            auto t = triangles.begin();
            while (t != triangles.end())
            {
                vector<vec2> sharedPoints;
                for (vec2 a : p.points)
                {
                    for (vec2 b : t->points)
                    {
                        if (a == b)
                        {
                            sharedPoints.push_back(b);
                        }
                    }
                }
                if (sharedPoints.size() > 2)
                {
                    //push back forbidden edge here
                    fedge->insert({ sharedPoints[0], sharedPoints[1] });
                    fedge->insert({ sharedPoints[1], sharedPoints[2] });
                    triangles.erase(t);
                }
                else
                {
                    t++;
                }
                
            }
        }
    }

    for (Triangle& t : triangles) //TODO: not needed once you're not rendering the navmesh
    {
        if (!counterClockwise(t))
        {
            vec2 temp = t.points[1];
            t.points[1] = t.points[2];
            t.points[2] = temp;
        }
    }
    return triangles;





    //TODO: reads in the texture file, makes the sobel image, translates that into edges and vertices, triangularizes those points,  with that one method
    //then writes those triangles to a file of some sort
}


Map::Map(string path, vec2 dims)
{
    printf("path: %s\n", path.c_str());
    unordered_set<Edge> fedges = unordered_set<Edge>();
    vector<Triangle> navMeshTris = generateNavMeshVerts(path + "navMesh.svg", "outFile.json", dims, &fedges);
    this->navMesh = NavMesh(navMeshTris, fedges, dims.x, dims.y);
    printf("made triangle tree\n");
    //this->navMesh.getTriangle(vec2(1)).print("Encasing 1,1: ");

    this->dims = dims;
    this->texture = makeTexture(path + "mapImage.png");

    this->vertices = {
        {vec2(0, 0), vec2(0, 0)},   //bottom left
        {vec2(dims.x, 0), vec2(1, 0)},  //bottom right
        {vec2(0, dims.y), vec2(0, 1)}, //top left
        {vec2(dims.x, dims.y), vec2(1, 1)} //top right
    };
    this->indices = { 0, 2, 1, 2, 3, 1 };
    setupBuffer();
    //this->texture = makeTexture(path + "mapImage.png");
    //this->navMesh = makeNavMesh(path + "navMesh.png");
}

Map::~Map()
{

}


vector<vec2> Map::getPath(vec2 start, vec2 end)
{
    return this->navMesh.getPath(start, end);
}

void Map::draw(Shader& shader)
{
    this->navMesh.draw(shader);


    glActiveTexture(GL_TEXTURE0);

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, this->texture.id);

    shader.setBool("outline", false);
    shader.setVecFour("outlineColor", vec4(1.0f, 1.0f, 0.0f, 1.0f));

    shader.setBool("ignoreAlpha", false);
    shader.setVecThree("tintRatio", vec3(0.0f));

    shader.setVecThree("tint", vec3(1.0f));
    shader.setMatFour("transform", mat4(1.0f));

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_LINES, 0, vertices.size());

    glBindVertexArray(0);



    //TODO:
}

void Map::setupBuffer()
{

    //printf("setting up mesh");
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));

    //printf("there are %i indices and %i vertices\n", indices.size(), vertices.size());

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}