#pragma once

#include <vector>

namespace GameClones
{
	namespace Sudoku
	{
		struct SudokuCell
		{
			size_t row = 0;
			size_t column = 0;

			char character = ' ';

			bool isModifiable = true;
		};

		class SudokuGrid
		{
		private:
			size_t m_numRows, m_numColumns;
			std::vector<SudokuCell> m_grid;
			
		public:
			SudokuGrid();
			SudokuGrid(int numRows, int numColumns);
			~SudokuGrid();

			void GetSize(size_t& outNumRows, size_t& outNumColumns) const;

			void ClearGrid();

			void SetCharacterAt(const size_t& row, const size_t& column, const char& character);
			char GetCharacterAt(const size_t& row, const size_t& column) const;

			void SetCellModifiableFlag(const size_t& row, const size_t& column, const bool& isModifiable);
			bool IsCellModifiable(const size_t& row, const size_t& column) const;

		private:
			bool IsValidLocation(const size_t& row, const size_t& column) const;

			int Get1DIndexFrom2D(const size_t& row, const size_t& column) const;
		};
	}
}