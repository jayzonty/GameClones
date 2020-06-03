#pragma once

#include <vector>

namespace GameClones
{
	namespace Sudoku
	{
		struct SudokuGridLocation
		{
			size_t row = 0;
			size_t column = 0;

			SudokuGridLocation() : SudokuGridLocation(0, 0)
			{
			}

			SudokuGridLocation(const size_t& r, const size_t& c)
			{
				row = r;
				column = c;
			}
		};

		class SudokuGrid
		{
		private:
			size_t m_numRows, m_numColumns;
			std::vector<char> m_grid;
			
		public:
			SudokuGrid();
			SudokuGrid(int numRows, int numColumns);
			~SudokuGrid();

			void GetSize(size_t& outNumRows, size_t& outNumColumns) const;

			void ClearGrid();

			void SetCharacterAt(const size_t& row, const size_t& column, const char& character);
			char GetCharacterAt(const size_t& row, const size_t& column) const;

		private:
			bool IsValidLocation(const size_t& row, const size_t& column) const;

			int Get1DIndexFrom2D(const size_t& row, const size_t& column) const;
			SudokuGridLocation UnflattenIndex(const size_t& flatIndex) const;
		};
	}
}