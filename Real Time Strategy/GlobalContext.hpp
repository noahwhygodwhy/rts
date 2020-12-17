#ifndef GLOBAL_CONTEXT_H
#define GLOBAL_CONTEXT_H

#include <glm/glm.hpp>
#include "SelectionBox.hpp"
#include "Map.hpp"


using namespace std;
using namespace glm;

namespace GLBL
{
	extern Map map;
	extern SelectionBox sb;
	extern vector<Entity*> things;
	extern unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;

}

#endif