
#include "Renderer.hpp"
#include "Map.hpp"
#include "tinyxml2.h"
#include "Polygon.hpp"
#include <array>
#include "Sobel.hpp"
#include "Delaunay.hpp"
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
    unsigned char* sobelImage = makeSobalImage(originalImage, width, height, NUM_CHANNELS);
    saveImage("SOBEL_IMAGE8325.png", sobelImage, width, height, NUM_CHANNELS);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sobelImage);
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

void generateNavMeshVerts(string inFilePath, string outFilePath)
{
    vector<Polygon> shapes;
    tinyxml2::XMLDocument theFile;
    theFile.LoadFile(inFilePath.c_str());
    tinyxml2::XMLElement* g = theFile.FirstChildElement("svg")->FirstChildElement("g");
    for (tinyxml2::XMLElement* ele = g->FirstChildElement(); ele; ele = ele->NextSiblingElement())
    {
        vector<string> pointStrings = splitOnSpaces(ele->FindAttribute("points")->Value());
        vector<int> pointInts;
        for (string s : pointStrings)
        {
            pointInts.push_back(stoi(s));
        }
        shapes.push_back(Polygon(pointInts));
    }
    vector<vec2> allPoints;
    for (Polygon x : shapes)
    {
        allPoints.insert(allPoints.end(), x.points.begin(), x.points.end());
    }
    vector<array<vec2, 3>> triangles = delaunay(allPoints);

    array<vec2, 3> superTriangle = { mins, vec2(mins.x, mins.y + ((maxs.y - mins.y) * 2)), vec2(mins.x + ((maxs.x - mins.x) * 2), mins.y) };
    vector<array<vec2, 3>> triangles; //probably not guarenteed to be CCW






    //TODO: reads in the texture file, makes the sobel image, translates that into edges and vertices, triangularizes those points,  with that one method
    //then writes those triangles to a file of some sort
}


Map::Map(string path, vec2 dims)
{
    this->dims = dims;
    this->texture = makeNavMesh(path + "navMeshb.png");
    //this->texture = makeTexture(path + "navMesh.png");
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


void Map::draw(Shader& shader)
{
    glActiveTexture(GL_TEXTURE0);

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, this->texture.id);

    shader.setBool("outline", false);
    shader.setVecFour("outlineColor", vec4(1.0f, 1.0f, 0.0f, 1.0f));

    shader.setBool("ignoreAlpha", false);
    shader.setVecThree("tintRatio", vec3(0.0f, 0.0f, 0.0f));
    shader.setVecThree("tint", vec3(1.0f, 0.0f, 0.0f));
    shader.setMatFour("transform", mat4(1.0f));

    glBindVertexArray(this->VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
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