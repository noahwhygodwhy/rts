
#include "Mesh.hpp"

using namespace std;
using namespace glm;


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
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

void Mesh::draw(Shader& shader, glm::mat4 transform)
{

    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    //printf("There are %i textures\n", textures.size());
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);//i love that this works

        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
        {
            number = to_string(diffuseNum++);
        }
        else if (name == "texture_specular")
        {
            number = to_string(specularNum++);
        }
        else if (name == "texture_normal")
        {
            number = to_string(normalNum++);
        }
        else if (name == "texture_height")
        {
            number = to_string(heightNum++);
        }
        else
        {
            printf("ERROR IN DRAW(), NO TEXTURE TYPE");
            exit(-1);
        }

        shader.setInt(("material." + name + number).c_str(), i);

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);

    }
    shader.setMatFour("transform", transform);


    glBindVertexArray(VAO);


    shader.setBool("hitbox", false);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


    //shader.setBool("hitbox", true);
    //glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);


    //glActiveTexture(GL_TEXTURE0);
}

vector<Vertex>* Mesh::getVertices()
{
    return &this->vertices;
}