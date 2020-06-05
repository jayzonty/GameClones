#pragma once

#ifndef ___Common_Color_h___
#define ___Common_Color_h___

#include <cstdint>

namespace Common {
	class Color {
	public:
		static const Color White;
		static const Color Black;
		static const Color Gray;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;
		static const Color Orange;
		static const Color Brown;
		static const Color Pink;
		static const Color Clear;
		
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color(unsigned char red, unsigned char green,
			unsigned char blue, unsigned char alpha);

		Color(unsigned char color);

		Color();

		Color(const Color& other);

		Color& operator=(const Color& other);
	};
}

#endif // ___Common_Color_h___
