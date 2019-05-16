#pragma once

#ifndef ___Common_Rect_h___
#define ___Common_Rect_h___

namespace Common {
	class Rect2D {
	public:
		float x;
		float y;
		float width;
		float height;

		Rect2D(float rectX, float rectY, float rectWidth, float rectHeight) {
			x = rectX;
			y = rectY;
			width = rectWidth;
			height = rectHeight;
		}

		Rect2D() : Rect2D(0.0f, 0.0f, 0.0f, 0.0f) {
		}

		bool ContainsPoint(float px, float py) const {
			return (x <= px && px <= x + width) &&
				(y <= py && py <= y + height);
		}

		bool Intersects(const Rect2D& rect) const {
			return Intersects(rect) || rect.Intersects(*this);
		}

		bool Intersects(const float& rectX, 
			const float& rectY, 
			const float& rectWidth, 
			const float& rectHeight) const {
			Rect2D rect(rectX, rectY, rectWidth, rectHeight);
			return Intersects(rect);
		}
	};
}

#endif // ___Common_Rect_h___
