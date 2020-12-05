
#include "Renderer.hpp"
#include "Map.hpp"
#include "tinyxml2.h"

using namespace glm;
using namespace std;


float sobelX[3][3] = { {1.0, 0.0, -1.0},{2.0, 0.0, -2.0}, {1.0, 0.0, -1.0} };
float sobelY[3][3] = { {1.0, 2.0, 1.0},{0.0, 0.0, 0.0}, {-1.0, -2.0, -1.0} };



double sobel(float sobelMat[3][3], unsigned char* imageData, int width, int height, int nrChannels, int pixelOffset, int colorOffset)
{
    int sum = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int yAdjust = (i - 1) * (width * NUM_CHANNELS);
            int xAdjust = (j - 1) * (NUM_CHANNELS);
            sum += sobelMat[i][j] * *(imageData + pixelOffset + yAdjust + xAdjust + colorOffset);
        }
    }
    return (double)sum;
}


unsigned char* makeSobalImage(unsigned char* originalImage, int width, int height, int nrChannels)
{
    unsigned char* sobelImage = new unsigned char[(width * height * NUM_CHANNELS)+1];

    int sumX = 0;
    int sumY = 0;
    int sum;
    int maxSobalVal = 0;


    for (int i = 0; i < width * height; i++)
    {
        int pixelOffset = i * NUM_CHANNELS;
        int y = i / width;
        int x = i % height;


        if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
        {
            *(sobelImage + pixelOffset + 0) = 0;
            *(sobelImage + pixelOffset + 1) = 0;
            *(sobelImage + pixelOffset + 2) = 0;
            *(sobelImage + pixelOffset + 3) = 255;
        }
        else
        {
            double rsx = sobel(sobelX, originalImage, width, height, NUM_CHANNELS, pixelOffset, 0);
            double rsy = sobel(sobelY, originalImage, width, height, NUM_CHANNELS, pixelOffset, 0);
            double rrealSum = sqrt((rsx * rsx) + (rsy * rsy));

            double gsx = sobel(sobelX, originalImage, width, height, NUM_CHANNELS, pixelOffset, 1);
            double gsy = sobel(sobelY, originalImage, width, height, NUM_CHANNELS, pixelOffset, 1);
            double grealSum = sqrt((gsx * gsx) + (gsy * gsy));

            double bsx = sobel(sobelX, originalImage, width, height, NUM_CHANNELS, pixelOffset, 2);
            double bsy = sobel(sobelY, originalImage, width, height, NUM_CHANNELS, pixelOffset, 2);
            double brealSum = sqrt((bsx * bsx) + (bsy * bsy));

            int avgSum = (int)((rrealSum + grealSum + brealSum) / 765.0f);

            maxSobalVal = std::max(maxSobalVal, avgSum);

            for (int colorOffset = 0; colorOffset < 3; colorOffset++)
            {
                *(sobelImage + pixelOffset + colorOffset) = avgSum;
            }
            *(sobelImage + pixelOffset + 3) = 255;
            
        }
    }

    for (int i = 0; i < width * height; i++)
    {
        int pixelOffset = i * NUM_CHANNELS;
        int y = i / width;
        int x = i % height;
        for (int j = 0; j < 3; j++)
        {
            *(sobelImage + pixelOffset + j) = *(sobelImage + pixelOffset + j) / maxSobalVal * 255;
        }

    }

    return sobelImage;
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
    vector<vec2> points;
    tinyxml2::XMLDocument theFile;
    theFile.LoadFile(inFilePath.c_str());
    //printf(" first child element: %s\n", theFile.FirstChildElement("svg")->FirstChildElement("g"));
    tinyxml2::XMLElement* g = theFile.FirstChildElement("svg")->FirstChildElement("g");
    for (tinyxml2::XMLElement* ele = g->FirstChildElement(); ele; ele = ele->NextSiblingElement())
    {
        string pointString = ele->FindAttribute("points")->Value();

        printf("%s\n", ele->FindAttribute("points")->Value());
    }
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