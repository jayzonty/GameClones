#include "Color.h"

namespace Common
{
	const Color Color::White = Color(255, 255, 255, 255);
	const Color Color::Black = Color(0, 0, 0, 0);
	const Color Color::Gray = Color(128, 128, 128, 255);
	const Color Color::Red = Color(255, 0, 0, 255);
	const Color Color::Green = Color(0, 255, 0, 255);
	const Color Color::Blue = Color(0, 0, 255, 255);
	const Color Color::Yellow = Color(255, 255, 0, 255);
	const Color Color::Cyan = Color(0, 255, 255, 255);
	const Color Color::Magenta = Color(255, 0, 255, 255);
	const Color Color::Orange = Color(255, 165, 0, 255);
	const Color Color::Brown = Color(165, 42, 42, 255);
	const Color Color::Pink = Color(255, 192, 203, 255);
	const Color Color::Clear = Color(0, 0, 0, 0);

	Color::Color(unsigned char red,
		unsigned char green,
		unsigned char blue,
		unsigned char alpha)
		: r(red), g(green), b(blue), a(alpha)
	{
	}

	Color::Color(unsigned char color) : Color(color, color, color, color)
	{
	}

	Color::Color() : Color(255)
	{
	}

	Color::Color(const Color& other) : Color(other.r, other.g, other.b, other.a)
	{
	}

	Color& Color::operator=(const Color& other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;

		return *this;
	}
}