#pragma once

#ifndef ___Common_Rect_h___
#define ___Common_Rect_h___

namespace Common {
	template <class T>
	class Rect {
	public:
		T x;
		T y;
		T width;
		T height;

		Rect(T rectX, T rectY, T rectWidth, T rectHeight) {
			x = rectX;
			y = rectY;
			width = rectWidth;
			height = rectHeight;
		}

		Rect() {
		}

		bool ContainsPoint(T px, T py) const {
			return (x <= px && px <= x + width) &&
				(y <= py && py <= y + height);
		}

		bool Intersects(const Rect& rect) const {
			return Intersects(rect) || rect.Intersects(*this);
		}

		bool Intersects(const T& rectX,
			const T& rectY,
			const T& rectWidth,
			const T& rectHeight) const {
			Rect rect(rectX, rectY, rectWidth, rectHeight);
			return Intersects(rect);
		}
	};
}

#endif // ___Common_Rect_h___
