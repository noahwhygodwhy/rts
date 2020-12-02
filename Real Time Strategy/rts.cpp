#include <stdio.h>
#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include "Entity.hpp"
#include "Shader.hpp"

using namespace std;
namespace fs = std::filesystem;

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
	return Texture();
}


int main(void)
{
	//TODO: generate a texture map for each folder (which will be each entity type)


	//TODO: this is still messed up. hash function maybe?



	fs::directory_iterator syst = std::filesystem::directory_iterator("./resources/entityTextures");

	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;
	for (fs::directory_entry entityTextures : syst)
	{
		unordered_map<textureAttributes, vector<Texture>*> entityTextureMap;
		printf("%i\n", entityTextureMap.size());
		for (fs::directory_entry textureFile : std::filesystem::directory_iterator(entityTextures.path()))
		{
			vector<string> pieces = split(textureFile.path().filename().stem().string(), '.');
			textureAttributes theAttribs = { stoi(pieces[1]), stoi(pieces[2]), pieces[0] };

			printf("doing %s\n", textureFile.path().filename().stem().string().c_str());

			Texture theTexture = makeTexture(textureFile.path().string());

			if (entityTextureMap.count(theAttribs)==0)
			{
				entityTextureMap[theAttribs] = new vector<Texture>();
			}
			entityTextureMap[theAttribs]->emplace(entityTextureMap[theAttribs]->begin() + stoi(pieces[3]), theTexture);
		}
		entityTextureMapMap[entityTextures.path().filename().string()] = entityTextureMap;
	}
	
	exit(0);
	printf("###starting\n");
	Renderer r = Renderer(1920, 1080);
	printf("###initializing\n");
	r.initialize();

	Entity e = Entity(vec2(0, 0), 10, 10, Controller(), entityTextureMapMap["basicEntity"]);

	printf("###running\n");
	r.run();
	printf("###ending\n");

}