#ifndef MAP_H
#define MAP_H

#include <string>

using namespace std;



class Map
{
public:
	Map(string path, vec2 dims);
	Map() {}
	void draw(Shader& shader);

	vec2 dims;
	~Map();

private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	void setupBuffer();
	Texture texture;
	vector<vec3> navMesh;

};

#endif