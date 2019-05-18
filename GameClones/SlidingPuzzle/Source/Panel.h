#pragma once

#ifndef ___GameClones_SlidingPuzzle_Panel_h___
#define ___GameClones_SlidingPuzzle_Panel_h___

#include "Common/RectangleShape.h"
#include "Common/Text.h"
#include "Common/RenderTarget.h"
#include "Common/Font.h"

namespace GameClones {
	namespace SlidingPuzzle {
		class Panel {
			Common::RectangleShape m_shape;
			Common::Text m_text;

			int m_id;

		public:
			Panel(int id);
			~Panel();

			int GetId() const;

			void Draw(Common::RenderTarget& renderTarget);

			void SetPosition(int x, int y);
			void SetSize(unsigned int width, unsigned int height);

			void SetFont(Common::Font& font);

		private:
			void UpdateTextPosition();
		};
	}
}

#endif // ___GameClones_SlidingPuzzle_Cell_h___
