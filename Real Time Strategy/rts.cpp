#include <stdio.h>
#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include "Entity.hpp"
#include "SelectionBox.hpp"
#include "Map.hpp"
#include "Shader.hpp"
#include "Map.hpp"
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

	printf("###initialized, adding things\n");
	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap = getEntityTextures("./resources/entityTextures");

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Entity* e = new Entity(vec2(i*40, j*80), vec2(40, 80), 15, Controller(), entityTextureMapMap["basicEntity"]);
			r.addEntity(e);
		}
	}*/

	r.map = Map("./resources/maps/map1/", vec2(8000, 8000));
	r.sb = SelectionBox(entityTextureMapMap["selection"]);

	printf("###running\n");
	r.run();
	printf("###ending\n");

}