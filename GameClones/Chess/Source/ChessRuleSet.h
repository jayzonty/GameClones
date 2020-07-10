#pragma once

#include <vector>

#include "Common/Vector.h"

#include "ChessBoard.h"
#include "ChessPiece.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessRuleSet
		{
		public:
			ChessRuleSet() = default;
			~ChessRuleSet() = default;

			virtual void GetPossibleCells(ChessPiece* chessPiece, const Common::Vector2& piecePosition, ChessBoard& chessBoard, std::vector<Common::Vector2>& outResult) = 0;
			virtual int GetWinner(const ChessBoard& chessBoard) = 0;
		};
	}
}