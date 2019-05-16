#pragma once

#ifndef ___Common_Drawable_h___
#define ___Common_Drawable_h___

#include "Matrix.h"

namespace Common {
	class Drawable {
	public:
		Drawable() {
		}

		virtual ~Drawable() {
		}

		virtual void Draw(const Matrix& transformationMatrix) = 0;
	};
}

#endif // ___Common_Drawable_h___
