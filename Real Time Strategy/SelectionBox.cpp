#include "SelectionBox.hpp"




SelectionBox::SelectionBox(unordered_map<textureAttributes, vector<Texture>*> textures)
{
    this->textures = textures;
    this->textureState = { 0, 0, "selection" };
    this->faceIndices = { 0, 3, 1, 1, 3, 2 };
    this->edgeIndices = { 0, 3, 3, 2, 2, 1, 1, 0 };

}

SelectionBox::~SelectionBox()
{
}

vector<Vertex> SelectionBox::makeSquareVertices(vec2 pointA, vec2 pointB)
{
    vec2 a = glm::min(pointA, pointB);
    vec2 b = glm::max(pointA, pointB);
    vector<Vertex> toReturn;
    toReturn.push_back(Vertex{ vec2(a.x, a.y), vec2(0, 0) });
    toReturn.push_back(Vertex{ vec2(b.x, a.y), vec2(0, 0) });
    toReturn.push_back(Vertex{ vec2(b.x, b.y), vec2(0, 0) });
    toReturn.push_back(Vertex{ vec2(a.x, b.y), vec2(0, 0) });
    return toReturn;
}

void SelectionBox::setupBuffer()
{
    //printf("setting up mesh");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &FaceEBO);
    glGenBuffers(1, &EdgeEBO);

    //printf("there are %i indices and %i vertices\n", indices.size(), vertices.size());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FaceEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(unsigned int), &faceIndices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EdgeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(unsigned int), &edgeIndices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}


void SelectionBox::draw(Shader& shader)
{
    if (this->active)
    {
        mat4 transform = mat4(1);

        glActiveTexture(GL_TEXTURE0);

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, textures[this->textureState]->at(this->textureAnimationStep / ANIMATION_SLOWDOWN_FACTOR).id);

        shader.setMatFour("transform", transform);

        glBindVertexArray(VAO);

        glLineWidth(2.0f);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FaceEBO);
        glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EdgeEBO);
        glDrawElements(GL_LINES, edgeIndices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}