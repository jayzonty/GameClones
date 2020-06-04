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

			size_t m_selectedRow;
			size_t m_selectedColumn;

		public:
			SudokuGridRenderer();
			~SudokuGridRenderer();

			void SetSelectedCell(const size_t& row, const size_t& column);

			void RenderSudokuGrid(Common::RenderTarget& renderTarget, const SudokuGrid& sudokuGrid, const float& cellSize);
		};
	}
}