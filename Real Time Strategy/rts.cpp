#include <stdio.h>
#include "UtilityFunctions.hpp"
#include "Renderer.hpp"
#include <iostream>
#include <filesystem>
#include "Entity.hpp"
#include "SelectionBox.hpp"
#include "Map.hpp"
#include "Shader.hpp"
#include "Delaunay.hpp"
#include "GlobalContext.hpp"



#include <set>
#include <queue>
#include <random>




using namespace std;
namespace fs = std::filesystem;



std::vector<std::string> split(std::string s, char c)
{
	std::stringstream toSplit(s);
	std::string segment;
	std::vector<std::string> toReturn;
	while (getline(toSplit, segment, c))
	{
		toReturn.push_back(segment);
	}
	return toReturn;
}

unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> getEntityTextures(string path)
{
	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;
	fs::directory_iterator syst = std::filesystem::directory_iterator(path);
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


	/*vector<vec2> points = { vec2(0), vec2(8000, 0), vec2(0, 8000), vec2(8000) };


	vector<Triangle> t = delaunay(points);
	printf("===============\n");
	for (Triangle x : t)
	{
		x.print();
	}


	exit(0);*/

	/*vector<vec2> points = { vec2(0, 0), vec2(1000, 0), vec2(500, 500)};
	vector<Triangle> t = delaunay(points);
	printf("===============\n");
	for (Triangle x : t)
	{
		x.print();
	}
	printf("===============\n");
	addAPoint(t, vec2(500, 0));
	for (Triangle x : t)
	{
		x.print();
	}
	printf("===============\n");
	exit(0);*/

	printf("###starting\n");
	Renderer r = Renderer(1920, 1080);
	printf("###initializing\n");
	r.initialize();

	printf("###initialized, adding things\n");
	GLBL::entityTextureMapMap = getEntityTextures(".\\resources\\entityTextures");

	/*GLBL::things.push_back(new Entity(vec2(120,240), vec2(120, 240), 15, Controller(), GLBL::entityTextureMapMap["basicEntity"]));

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GLBL::things.push_back(new Entity(vec2(i*40, j*80), vec2(40, 80), 15, Controller(), GLBL::entityTextureMapMap["basicEntity"]));
		}
	}*/
	GLBL::map = Map(".\\resources\\maps\\map1\\", vec2(8000, 8000)); //TODO: need to scale the triangularization from image size to given dims
	GLBL::sb = SelectionBox(GLBL::entityTextureMapMap["selection"]);

	printf("###running\n");
	r.run();
	printf("###ending\n");

}