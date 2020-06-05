#include "SudokuGrid.h"

namespace GameClones
{
	namespace Sudoku
	{
		SudokuGrid::SudokuGrid()
			: SudokuGrid(9, 9)
		{
		}

		SudokuGrid::SudokuGrid(int numRows, int numColumns)
			: m_numRows(numRows)
			, m_numColumns(numColumns)
			, m_grid()
		{
			m_grid.resize(numRows * numColumns);
		}

		SudokuGrid::~SudokuGrid()
		{
		}

		void SudokuGrid::GetSize(size_t & outNumRows, size_t & outNumColumns) const
		{
			outNumRows = m_numRows;
			outNumColumns = m_numColumns;
		}

		void SudokuGrid::ClearGrid()
		{
			for (size_t i = 0; i < m_grid.size(); ++i)
			{
				m_grid[i].character = ' ';
				m_grid[i].isModifiable = true;
			}
		}

		void SudokuGrid::SetCharacterAt(const size_t & row, const size_t & column, const char & character)
		{
			if (IsValidLocation(row, column) && IsCellModifiable(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				m_grid[index].character = character;
			}
		}

		char SudokuGrid::GetCharacterAt(const size_t & row, const size_t & column) const
		{
			if (IsValidLocation(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				return m_grid[index].character;
			}

			return ' ';
		}

		void SudokuGrid::SetCellModifiableFlag(const size_t & row, const size_t & column, const bool & isModifiable)
		{
			if (IsValidLocation(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				m_grid[index].isModifiable = isModifiable;
			}
		}

		bool SudokuGrid::IsCellModifiable(const size_t & row, const size_t & column) const
		{
			if (IsValidLocation(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				return m_grid[index].isModifiable;
			}

			return false;
		}

		bool SudokuGrid::IsValidLocation(const size_t & row, const size_t & column) const
		{
			return (0 <= row && row < m_numRows)
				&& (0 <= column && column < m_numColumns);
		}

		int SudokuGrid::Get1DIndexFrom2D(const size_t & row, const size_t & column) const
		{
			size_t ret = row * m_numColumns + column;
			return ret;
		}
	}
}