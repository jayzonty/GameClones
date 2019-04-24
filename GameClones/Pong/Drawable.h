#pragma once

#ifndef ___GameClones_IDrawable_h___
#define ___GameClones_IDrawable_h___

#include <SFML/Window.hpp>

namespace GameClones {
	class Drawable {
	public:
		virtual void Draw(sf::RenderTarget& renderTarget) = 0;
	};
}

#endif // ___GameClones_IDrawable_h___
