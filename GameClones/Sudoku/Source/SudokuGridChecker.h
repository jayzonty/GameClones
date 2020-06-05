#pragma once

#include "SudokuGrid.h"

namespace GameClones
{
	namespace Sudoku
	{
		class SudokuGridChecker
		{
		private:

		public:
			SudokuGridChecker();
			~SudokuGridChecker();

			bool IsSolved(const SudokuGrid& grid);
		};
	}
}
