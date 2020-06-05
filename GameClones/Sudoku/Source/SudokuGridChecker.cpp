#include "SudokuGridChecker.h"

#include <vector>

namespace GameClones
{
	namespace Sudoku
	{
		SudokuGridChecker::SudokuGridChecker()
		{
		}

		SudokuGridChecker::~SudokuGridChecker()
		{
		}

		bool SudokuGridChecker::IsSolved(const SudokuGrid & grid)
		{
			size_t numRows = 0, numColumns = 0;
			grid.GetSize(numRows, numColumns);

			std::vector<bool> digitFlags;
			digitFlags.resize(9, false);

			// Check if all numbers are in all rows
			for (size_t row = 0; row < numRows; ++row)
			{
				for (size_t i = 0; i < digitFlags.size(); ++i)
				{
					digitFlags[i] = false;
				}

				for (size_t column = 0; column < numColumns; ++column)
				{
					char character = grid.GetCharacterAt(row, column);
					if (character == ' ')
					{
						return false;
					}

					digitFlags[character - '1'] = true;
				}

				for (size_t i = 0; i < digitFlags.size(); ++i)
				{
					if (!digitFlags[i])
					{
						return false;
					}
				}
			}

			// Check if all numbers are in all columns
			for (size_t column = 0; column < numColumns; ++column)
			{
				for (size_t i = 0; i < digitFlags.size(); ++i)
				{
					digitFlags[i] = false;
				}

				for (size_t row = 0; row < numRows; ++row)
				{
					char character = grid.GetCharacterAt(row, column);
					if (character == ' ')
					{
						return false;
					}

					digitFlags[character - '1'] = true;
				}

				for (size_t i = 0; i < digitFlags.size(); ++i)
				{
					if (!digitFlags[i])
					{
						return false;
					}
				}
			}

			return true;
		}
	}
}
