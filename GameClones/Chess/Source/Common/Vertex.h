#ifndef ___Common_Vertex_h___
#define ___Common_Vertex_h___

#include <cstdint>

#include "Color.h"

namespace Common
{
	struct Vertex
	{
		float x;
		float y;
		float z;

		float u;
		float v;

		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
}

#endif // ___Common_Vertex_h___
