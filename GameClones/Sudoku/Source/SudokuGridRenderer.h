#pragma once

#include "Common/Font.h"
#include "Common/RectangleShape.h"
#include "Common/RenderTarget.h"
#include "Common/Text.h"

#include "SudokuGrid.h"

namespace GameClones
{
	namespace Sudoku
	{
		class SudokuGridRenderer
		{
		private:
			Common::RectangleShape m_rectangleShape;
			Common::Font m_font;
			Common::Text m_text;

		public:
			SudokuGridRenderer();
			~SudokuGridRenderer();

			void RenderSudokuGrid(Common::RenderTarget& renderTarget, const SudokuGrid& sudokuGrid, const float& cellSize);
		};
	}
}