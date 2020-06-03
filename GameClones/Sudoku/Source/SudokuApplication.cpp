#include "SudokuApplication.h"

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
		}

		void SudokuApplication::Draw()
		{
			m_sudokuGridRenderer.RenderSudokuGrid(m_renderTarget, m_sudokuGrid, 100.0f);
		}

		void SudokuApplication::Cleanup()
		{
		}
	}
}
