#ifndef USEFUL_STRUCTS_H
#define USEFUL_STRUCTS_H


struct textureAttributes
{
	int x;
	int y;
	string action;
};

struct Vertex {
	vec2 position;
	vec2 texCoords;
};

struct Texture {
	unsigned int id = 0;
	string type;
	string path;
};

namespace std {
	template<>
	struct hash<textureAttributes> {
		inline size_t operator()(const textureAttributes& x) const
		{
			hash<string> stringHasher;
			hash<int> intHasher;

			size_t value = stringHasher(x.action) ^ intHasher(x.x) ^ intHasher(x.y); //= your hash computations over x
			return value;
		}
	};

	template<>
	struct equal_to<textureAttributes> {
		inline bool operator()(const textureAttributes& a, const textureAttributes& b) const
		{
			return a.x == b.x && a.y == b.y && a.action == b.action;
		}
	};
}

#endif