
#include "Renderer.hpp"
#include "Map.hpp"

using namespace glm;
using namespace std;


float sobelX[3][3] = { {1.0, 0.0, -1.0},{2.0, 0.0, -2.0}, {1.0, 0.0, -1.0} };
float sobelY[3][3] = { {1.0, 2.0, 1.0},{0.0, 0.0, 0.0}, {-1.0, -2.0, -1.0} };


int sobel(float sobelMat[3][3], unsigned char* imageData, int width, int height, int x, int y, int colorOffset)
{
    int sum = 0;
    int pixelIndex = (x * 4) + ((x * y) * 4) + 1;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sum += sobelMat[i][j] * (pixelIndex + ((i - 1) * width * 4) + (j * 4));
        }
    }
    return sum;
}


unsigned char* makeSobalImage(unsigned char* originalImage, int width, int height, int nrChannels)
{

    unsigned char* sobelImage = new unsigned char[width * height * nrChannels];

    int sumX = 0;
    int sumY = 0;
    int sum;
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            for (int colorOffset = 0; colorOffset < 3; colorOffset++)
            {
                int sx = sobel(sobelX, originalImage, width, height, x, y, colorOffset);
                int sy = sobel(sobelY, originalImage, width, height, x, y, colorOffset);
                int realSum = sqrt((sx * sx) + (sy * sy));
                *(sobelImage + ((y * x) * nrChannels) + (x * nrChannels)) = realSum;
            }
        }
    }
    return sobelImage;
}

Texture makeNavMesh(string filePath)
{


    int width, height, nrChannels;
    unsigned char* originalImage = imageToBytes(filePath.c_str(), &width, &height, &nrChannels);

    printf("original image: %p\n", originalImage);

    printf("making sobel\n");
    //unsigned char* sobelImage = makeSobalImage(originalImage, width, height, nrChannels);
    printf("done sobel\n");


    /*for (int i = 0; i < width * height; i += 4)
    {
        ivec3 color(*(originalImage + i), *(originalImage + i + 1), *(originalImage + i + 2));
        int y = i / width;
        int x = i % width;
        if (color != ivec3(255))
        {
            printf("%i,%i,%i\n", color.x, color.y, color.z);
        }
    }*/

    Texture t;

    t.type = "diffuse";
    t.path = filePath;
    
    unsigned int texture;
    //printf("%p\n", &texture);
    glGenTextures(1, &texture);
    //printf("%i\n", texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, originalImage);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    t.id = texture;
    t.dims = vec2(width, height);
    t.channels = nrChannels;
    
    return t;
}



Map::Map(string path, vec2 dims)
{
    this->dims = dims;
    this->texture = makeNavMesh(path + "navMesh.png");
    this->texture = makeTexture(path + "mapImage.png");
    this->vertices = {
        {vec2(0, 0), vec2(0, 0)},   //bottom left
        {vec2(dims.x, 0), vec2(1, 0)},  //bottom right
        {vec2(0, dims.y), vec2(0, 1)}, //top left
        {vec2(dims.x, dims.y), vec2(1, 1)} //top right
    };
    this->indices = { 0, 1, 2, 2, 1, 3 };
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