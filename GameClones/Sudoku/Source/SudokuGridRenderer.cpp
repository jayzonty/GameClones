#include "SudokuGridRenderer.h"

#include <string>

#include "Common/Color.h"

namespace GameClones
{
	namespace Sudoku
	{
		SudokuGridRenderer::SudokuGridRenderer()
			: m_rectangleShape()
			, m_font()
			, m_text()
		{
			m_font.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");
			m_font.SetSize(50);
			m_text.SetFont(m_font);
			m_text.SetHorizontalAlignment(Common::Text::HorizontalAlignment::CENTER);

			m_rectangleShape.SetBorderColor(Common::Color::Black);
			m_rectangleShape.SetBorderSizes(1.0f, 0.0f, 1.0f, 0.0f);

			m_rectangleShape.SetFillColor(Common::Color::White);
		}

		SudokuGridRenderer::~SudokuGridRenderer()
		{
		}

		void SudokuGridRenderer::RenderSudokuGrid(Common::RenderTarget& renderTarget, const SudokuGrid & sudokuGrid, const float& cellSize)
		{
			size_t numRows = 0, numColumns = 0;
			sudokuGrid.GetSize(numRows, numColumns);

			m_rectangleShape.SetSize(cellSize, cellSize);


			for (size_t row = 0; row < numRows; ++row)
			{
				for (size_t column = 0; column < numColumns; ++column)
				{
					m_rectangleShape.SetPosition(column * cellSize, row * cellSize);
					renderTarget.Draw(m_rectangleShape);

					// Temporary: calculation for the row is because right now (0, 0) is at the bottom-left
					char characterAtCell = sudokuGrid.GetCharacterAt(numRows - row - 1, column);
					std::string str;
					str += characterAtCell;
					m_text.SetString(str);

					size_t textWidth = 0, textHeight = 0;
					m_text.ComputeSize(&textWidth, &textHeight);

					m_text.SetPosition(column * cellSize + (cellSize - textWidth) / 2.0f, row * cellSize + (cellSize - textHeight) / 2.0f);
					renderTarget.Draw(m_text);
				}
			}
		}
	}
}