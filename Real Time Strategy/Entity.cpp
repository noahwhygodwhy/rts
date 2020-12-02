#include "Entity.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


vector<Vertex> makeVertices(float width, float height)
{
    vector<Vertex> toReturn;
    toReturn.push_back({ vec3(0, 0, 0), vec3(0, 0, 1), vec2(0, 0) });       //bottom left
    toReturn.push_back({ vec3(width/100, 0, 0), vec3(0, 0, 1), vec2(1, 0) });//top left
    toReturn.push_back({ vec3(width/100, height/100, 0), vec3(0, 0, 1), vec2(1, 1) });//top right
    toReturn.push_back({ vec3(0, height/100, 0), vec3(0, 0, 1), vec2(0, 1) });//bottom right
    return toReturn;
}


string vec22String(ivec2 theVec)
{
    return to_string(theVec.x) + "." + to_string(theVec.y);
}

Entity::Entity(vec2 location, int width, int height, Controller c, const unordered_map<textureAttributes, vector<Texture>*>& textures)
{
    this->textures = textures;
    this->vertices = makeVertices(width, height);
    this->indices = {0, 3, 1, 1, 3, 2};
	this->location = location;
	this->orientation = ivec2(1, 0);
    this->textureState = { orientation.x, orientation.y, "standing" };
    this->textureAnimationStep = 0;
}

Entity::~Entity()
{

}

void Entity::setupBuffer()
{
    //printf("setting up mesh");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //printf("there are %i indices and %i vertices\n", indices.size(), vertices.size());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
void Entity::draw(Shader& shader)
{
    mat4 transform = glm::translate(mat4(1), vec3(this->location, 0));
    

    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    //printf("There are %i textures\n", textures.size());

    //TODO: this needs to not be a for loop
    //i needs to be the texture next up in the loop, an iterator looping through but
    //always starting at 0


    glActiveTexture(GL_TEXTURE0);//i love that this works

    string name = textures[this->textureState]->at(this->textureAnimationStep).type;

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, textures[this->textureState]->at(this->textureAnimationStep).id);
    
    shader.setMatFour("transform", transform);

    glBindVertexArray(VAO);

    shader.setBool("hitbox", false);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //shader.setBool("hitbox", true);
    //glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);


    glActiveTexture(GL_TEXTURE0);
}

void Entity::tick(float deltaTime)
{
    this->textureAnimationStep += 1;
    if (this->textureAnimationStep >= this->textures[textureState]->size())
    {
        this->textureAnimationStep == 0;
    }
}