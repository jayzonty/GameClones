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
			, m_selectedRow(0)
			, m_selectedColumn(0)
		{
			m_font.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");
			m_font.SetSize(50);
			m_text.SetFont(m_font);
			m_text.SetHorizontalAlignment(Common::Text::HorizontalAlignment::CENTER);

			m_rectangleShape.SetBorderColor(Common::Color::Black);

			m_rectangleShape.SetFillColor(Common::Color::White);
		}

		SudokuGridRenderer::~SudokuGridRenderer()
		{
		}

		void SudokuGridRenderer::SetSelectedCell(const size_t & row, const size_t & column)
		{
			m_selectedRow = row;
			m_selectedColumn = column;
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
					Common::Color cellColor = Common::Color::White;
					if (row == m_selectedRow && column == m_selectedColumn)
					{
						cellColor = Common::Color::Cyan;
					}
					m_rectangleShape.SetFillColor(cellColor);

					float leftBorderSize = 1.0f;
					float rightBorderSize = 0.0f;
					float topBorderSize = 1.0f;
					float bottomBorderSize = 0.0f;

					if (column > 0 && column % 3 == 0)
					{
						leftBorderSize = 2.0f;
					}
					if (row > 0 && row % 3 == 0)
					{
						topBorderSize = 2.0f;
					}
					m_rectangleShape.SetBorderSizes(leftBorderSize, rightBorderSize, topBorderSize, bottomBorderSize);

					float cellPositionX = column * cellSize;
					float cellPositionY = (numRows - row - 1) * cellSize;
					m_rectangleShape.SetPosition(cellPositionX, cellPositionY);
					renderTarget.Draw(m_rectangleShape);

					char characterAtCell = sudokuGrid.GetCharacterAt(row, column);
					std::string str;
					str += characterAtCell;
					m_text.SetString(str);

					size_t textWidth = 0, textHeight = 0;
					m_text.ComputeSize(&textWidth, &textHeight);

					m_text.SetPosition(cellPositionX + (cellSize - textWidth) / 2.0f, cellPositionY + (cellSize - textHeight) / 2.0f);
					renderTarget.Draw(m_text);
				}
			}
		}
	}
}