#include "Entity.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Delaunay.hpp"


static int idCounter = 0;
vector<Vertex> Entity::makeSquareVertices(float width, float height)
{
    vector<Vertex> toReturn;
    toReturn.push_back({ vec2(0, 0), vec2(0, 0) });       //bottom left
    toReturn.push_back({ vec2(width, 0), vec2(1, 0) });//top left
    toReturn.push_back({ vec2(width, height), vec2(1, 1) });//top right
    toReturn.push_back({ vec2(0, height), vec2(0, 1) });//bottom right
    return toReturn;
}

string vec22String(ivec2 theVec)
{
    return to_string(theVec.x) + "." + to_string(theVec.y);
}

Entity::Entity(vec2 location, vec2 dims, float hitboxRadius, Controller c, const unordered_map<textureAttributes, vector<Texture>*>& textures)
{
    this->textures = textures;
    this->vertices = this->makeSquareVertices(dims.x, dims.y);
    this->indices = {0, 3, 1, 1, 3, 2};
	this->location = location;
	this->orientation = ivec2(1, 0);
    this->textureState = { orientation.x, orientation.y, "walking" };
    this->textureAnimationStep = 0;
    this->dims = dims;
    this->hitboxRadius = hitboxRadius;
    this->selected = false;
    this->id = idCounter++;
    this->setupBuffer();

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
void Entity::draw(Shader& shader)
{


    glActiveTexture(GL_TEXTURE0);

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, textures[this->textureState]->at(this->textureAnimationStep / ANIMATION_SLOWDOWN_FACTOR).id);

    mat4 transform = glm::translate(mat4(1), vec3(this->location, 0));
    
    
    shader.setBool("outline", this->selected);
    shader.setVecFour("outlineColor", vec4(1.0f, 1.0f, 0.0f, 1.0f));

    shader.setBool("ignoreAlpha", false);
    shader.setVecThree("tintRatio", vec3(0.5f, 0.5f, 0.5f));
    shader.setVecThree("tint", vec3(0.0f, 0.0f, 0.0f));
    shader.setMatFour("transform", transform);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Entity::tick(float deltaTime)
{

    //TODO: Flocking? http://www.red3d.com/cwr/boids/
    //todo: navmesh a* https://www.gamedev.net/tutorials/programming/artificial-intelligence/navigation-meshes-and-pathfinding-r4880/


    this->textureAnimationStep += 1;
    if (this->textureAnimationStep > (this->textures[textureState]->size()* ANIMATION_SLOWDOWN_FACTOR)-1)
    {
        this->textureAnimationStep = 0;
    }


    if (this->targetType != TargetType::NONE)
    {
        vec2 placeToBe;
        if (this->targetType == TargetType::LOCATION)
        {
            placeToBe = *(vec2*)this->target;
        }
        if (this->targetType == TargetType::LOCATION)
        {
            placeToBe = ((Entity*)this->target)->location;
        }

    }
}


void Entity::setOrientation(vec2 newO)
{
    this->orientation = newO;
    this->textureAnimationStep = 0;
    this->textureState.x = newO.x;
    this->textureState.y = newO.y;
}

namespace std
{
    template <>
    struct hash<Entity>
    {
        size_t operator()(const Entity& k) const
        {
            return k.id;
        }
    };
}


void Entity::setTarget(vec2 targetLocation)
{
    this->targetType = TargetType::LOCATION;
    this->target = new vec2(targetLocation);
}
void Entity::setTarget(Entity* targetEntity)
{
    this->targetType = TargetType::ENTITY;
    this->target = targetEntity;
