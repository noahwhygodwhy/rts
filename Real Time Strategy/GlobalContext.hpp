#ifndef GLOBAL_CONTEXT_H
#define GLOBAL_CONTEXT_H

#include <glm/glm.hpp>
#include "SelectionBox.hpp"
#include "Map.hpp"


using namespace std;
using namespace glm;

//EVERYTHING must be static
namespace GLBL
{
	static Map map;
	static SelectionBox sb;
	static vector<Entity*> things;
	static unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;
}

#endif;