#include <stdio.h>
#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include "Entity.hpp"
#include "SelectionBox.hpp"
#include "Shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
namespace fs = std::filesystem;


unordered_map<string, unsigned int> loadedTextures;

vector<string> split(string s, char c)
{
	stringstream toSplit(s);
	string segment;
	vector<string> toReturn;
	while (getline(toSplit, segment, c))
	{
		toReturn.push_back(segment);
	}
	return toReturn;
}

Texture makeTexture(string filepath)
{
	Texture t;
	t.type = "diffuse";
	t.path = filepath;
	if (loadedTextures.count(filepath))
	{
		t.id = loadedTextures[filepath];
	}
	else
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);

		unsigned int texture;
		//printf("%p\n", &texture);
		glGenTextures(1, &texture);
		//printf("%i\n", texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		t.id = texture;
	}

	return t;
}


unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> getEntityTextures(string path)
{
	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;
	fs::directory_iterator syst = std::filesystem::directory_iterator("./resources/entityTextures");
	for (fs::directory_entry entityTextures : syst)
	{
		unordered_map<textureAttributes, vector<Texture>*> entityTextureMap;
		for (fs::directory_entry textureFile : std::filesystem::directory_iterator(entityTextures.path()))
		{
			vector<string> pieces = split(textureFile.path().filename().stem().string(), '.');
			textureAttributes theAttribs = { stoi(pieces[1]), stoi(pieces[2]), pieces[0] };

			Texture theTexture = makeTexture(textureFile.path().string());

			if (entityTextureMap.count(theAttribs) == 0)
			{
				entityTextureMap[theAttribs] = new vector<Texture>();
			}
			entityTextureMap[theAttribs]->emplace(entityTextureMap[theAttribs]->begin() + stoi(pieces[3]), theTexture);
		}
		entityTextureMapMap[entityTextures.path().filename().string()] = entityTextureMap;
	}
	return entityTextureMapMap;
}


int main(void)
{

	printf("###starting\n");
	Renderer r = Renderer(1920, 1080);
	printf("###initializing\n");
	r.initialize();

	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap = getEntityTextures("./resources/entityTextures");

	Entity* e = new Entity(vec2(0, 0), 40, 80, Controller(), entityTextureMapMap["basicEntity"]);
	Entity* e2 = new Entity(vec2(0, 0), 40, 80, Controller(), entityTextureMapMap["basicEntity"]);

	SelectionBox sb = SelectionBox(entityTextureMapMap["selection"]);

	r.addEntity(e);
	r.addEntity(e2);
	r.sb = sb;
	printf("###running\n");
	r.run();
	printf("###ending\n");

}