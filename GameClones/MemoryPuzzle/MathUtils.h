#pragma once

#ifndef ___GameClones_MathUtils_h___
#define ___GameClones_MathUtils_h___

namespace GameClones {
	namespace MathUtils {
		const double PI_D = atan(1.0) * 4.0;
		const float PI_F = atanf(1.0) * 4.0f;

		bool IsPointInsideAABBRectangle(float pointX, float pointY, float rectX, float rectY, float rectWidth, float rectHeight) {
			return (rectX <= pointX) && (pointX <= rectX + rectWidth) && (rectY <= pointY) && (pointY <= rectY + rectHeight);
		}

		bool DoRectanglesIntersect(
			float x1, float y1, float rect1Width, float rect1Height,
			float x2, float y2, float rect2Width, float rect2Height) {
			return IsPointInsideAABBRectangle(x1, y1, x2, y2, rect2Width, rect2Height) ||
				IsPointInsideAABBRectangle(x1 + rect1Width, y1, x2, y2, rect2Width, rect2Height) ||
				IsPointInsideAABBRectangle(x1 + rect1Width, y1 + rect1Height, x2, y2, rect2Width, rect2Height) ||
				IsPointInsideAABBRectangle(x1, y1 + rect1Height, x2, y2, rect2Width, rect2Height) ||
				IsPointInsideAABBRectangle(x2, y2, x1, y1, rect1Width, rect1Height) ||
				IsPointInsideAABBRectangle(x2 + rect2Width, y2, x1, y1, rect1Width, rect1Height) ||
				IsPointInsideAABBRectangle(x2 + rect2Width, y2 + rect2Height, x1, y1, rect1Width, rect1Height) ||
				IsPointInsideAABBRectangle(x2, y2 + rect2Height, x1, y1, rect1Width, rect1Height);
		}

		template <class T>
		T Clamp(T value, T min, T max) {
			if (value < min) {
				value = min;
			}
			else if (value > max) {
				value = max;
			}

			return value;
		}
	}
}

#endif // ___GameClones_MathUtils_h___
