#include "SudokuApplication.h"

#include "Common/Input.h"
#include "Common/Window.h"
#include "Common/WindowManager.h"

namespace GameClones
{
	namespace Sudoku
	{
		SudokuApplication::SudokuApplication()
			: BaseApplication("Sudoku")
			, m_renderTarget()
			, m_sudokuGrid()
			, m_sudokuGridRenderer()
			, m_selectedRow(0)
			, m_selectedColumn(0)
		{
		}

		SudokuApplication::~SudokuApplication()
		{
		}

		void SudokuApplication::SetupWindow(Common::Window* window)
		{
			window->SetSize(900, 900);
			window->SetTitle("Sudoku");

			m_renderTarget.SetSize(900, 900);
		}

		void SudokuApplication::Init()
		{
			// Temporary!
			// Random sudoku puzzle found online encoded as a long string
			const char* puzzle = "568 93  7|34   7  5| 975 46 3|7   125  | 19  876 | 8 7   3 | 2  358  | 512 63  |   9     ";
			for (size_t i = 0; i < 9; ++i)
			{
				for (size_t j = 0; j < 10; ++j)
				{
					if (j < 9)
					{
						size_t index = i * 10 + j;
						m_sudokuGrid.SetCharacterAt(i, j, puzzle[index]);
					}
				}
			}
		}

		void SudokuApplication::Update(float deltaTime)
		{
			if (Common::Input::IsPressed(Common::Input::LEFT_MOUSE))
			{
				int mouseX = 0, mouseY = 0;
				Common::Input::GetMousePosition(&mouseX, &mouseY);

				size_t numRows = 0, numColumns = 0;
				m_sudokuGrid.GetSize(numRows, numColumns);

				m_selectedRow = static_cast<size_t>(mouseY / GetCellSize());
				m_selectedColumn = static_cast<size_t>(mouseX / GetCellSize());

				m_sudokuGridRenderer.SetSelectedCell(m_selectedRow, m_selectedColumn);
			}
			// TODO: Should have a better way of doing this
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA0))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '0');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA1))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '1');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA2))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '2');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA3))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '3');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA4))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '4');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA5))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '5');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA6))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '6');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA7))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '7');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA8))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '8');
			}
			else if (Common::Input::IsPressed(Common::Input::Key::ALPHA9))
			{
				m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '9');
			}
		}

		void SudokuApplication::Draw()
		{
			m_sudokuGridRenderer.RenderSudokuGrid(m_renderTarget, m_sudokuGrid, GetCellSize());
		}

		void SudokuApplication::Cleanup()
		{
		}

		float SudokuApplication::GetCellSize() const
		{
			return 100.0f; // temporary
		}
	}
}
