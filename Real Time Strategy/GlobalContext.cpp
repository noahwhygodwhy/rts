#include "GlobalContext.hpp"

namespace GLBL
{
	Map map;
	SelectionBox sb;
	vector<Entity*> things;
	unordered_map<string, unordered_map<textureAttributes, vector<Texture>*>> entityTextureMapMap;
}