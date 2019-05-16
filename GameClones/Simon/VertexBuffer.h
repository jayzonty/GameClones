#pragma once

#ifndef ___Common_VertexBuffer_h___
#define ___Common_VertexBuffer_h___

#include "Vertex.h"

namespace Common {
	struct VertexBuffer {
		Vertex* vertices;
		int vertexCount;
	};
}

#endif // ___Common_VertexBuffer_h___
