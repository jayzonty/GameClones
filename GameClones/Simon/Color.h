#pragma once

#ifndef ___Common_Color_h___
#define ___Common_Color_h___

#include <cstdint>

namespace Common {
	class Color {
	public:
		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color(const unsigned char red, 
			const unsigned char green, 
			const unsigned char blue, 
			const unsigned char alpha) : 
			r(red), g(green), b(blue), a(alpha) {
		}

		Color(unsigned char color) : Color(color, color, color, color) {
		}

		Color() : Color(255) {
		}

		Color(const Color& other) : Color(other.r, other.g, other.b, other.a) {
		}

		Color& operator=(const Color& other) {
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;

			return *this;
		}
	};
}

#endif // ___Common_Color_h___
