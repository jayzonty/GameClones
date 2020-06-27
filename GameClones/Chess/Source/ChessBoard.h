#pragma once

#include <vector>

#include "ChessPiece.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessBoard
		{
		private:
			struct Cell
			{
				ChessPiece* piece = nullptr;
			};

			size_t m_numRows, m_numColumns;
			std::vector<Cell> m_grid;

		public:

			ChessBoard();
			ChessBoard(size_t numRows, size_t numColumns);
			~ChessBoard();

			void GetSize(size_t& outNumRows, size_t& outNumColumns) const;

			void ClearGrid();

			void SetPieceAt(const size_t& row, const size_t& column, ChessPiece* piece);
			ChessPiece* GetPieceAt(const size_t& row, const size_t& column) const;

		private:
			bool IsValidLocation(const size_t& row, const size_t& column) const;

			int Get1DIndexFrom2D(const size_t& row, const size_t& column) const;

		};
	}
}