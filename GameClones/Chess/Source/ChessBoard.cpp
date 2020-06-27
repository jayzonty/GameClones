#include "ChessBoard.h"

namespace GameClones
{
	namespace Chess
	{
		ChessBoard::ChessBoard()
			: ChessBoard(8, 8)
		{
		}

		ChessBoard::ChessBoard(size_t numRows, size_t numColumns)
			: m_numRows(numRows)
			, m_numColumns(numColumns)
			, m_grid()
		{
			m_grid.resize(numRows * numColumns);
		}

		ChessBoard::~ChessBoard()
		{
		}

		void ChessBoard::GetSize(size_t & outNumRows, size_t & outNumColumns) const
		{
			outNumRows = m_numRows;
			outNumColumns = m_numColumns;
		}

		void ChessBoard::ClearGrid()
		{
			for (size_t i = 0; i < m_grid.size(); ++i)
			{
				m_grid[i].piece = nullptr;
			}
		}

		void ChessBoard::SetPieceAt(const size_t & row, const size_t & column, ChessPiece* piece)
		{
			if (IsValidLocation(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				m_grid[index].piece = piece;
			}
		}

		ChessPiece* ChessBoard::GetPieceAt(const size_t & row, const size_t & column) const
		{
			if (IsValidLocation(row, column))
			{
				size_t index = Get1DIndexFrom2D(row, column);
				return m_grid[index].piece;
			}

			return nullptr;
		}

		bool ChessBoard::IsValidLocation(const size_t & row, const size_t & column) const
		{
			return (0 <= row && row < m_numRows)
				&& (0 <= column && column < m_numColumns);
		}

		int ChessBoard::Get1DIndexFrom2D(const size_t & row, const size_t & column) const
		{
			size_t ret = row * m_numColumns + column;
			return ret;
		}
	}
}