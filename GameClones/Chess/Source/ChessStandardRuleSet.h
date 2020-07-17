#pragma once

#include "ChessRuleSet.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessStandardRuleSet : public ChessRuleSet
		{
		public:
			ChessStandardRuleSet()
				: ChessRuleSet()
			{
			}

			~ChessStandardRuleSet() = default;

			void GetPossibleCells(ChessPiece* chessPiece, const Common::Vector2& piecePosition, ChessBoard& chessBoard, std::vector<Common::Vector2>& outResult) override
			{
				if (chessPiece == nullptr)
				{
					return;
				}

				int x = piecePosition.x;
				int y = piecePosition.y;
				
				size_t width = 0, height = 0;
				chessBoard.GetSize(width, height);

				if (chessPiece->GetType() == ChessPiece::Type::Pawn)
				{
					// Basic pawn movement
					int offset = chessPiece->GetPlayerIndex() == 0 ? 1 : -1;
					int y1 = y + offset;
					if ((0 <= y1 && y1 < height) && (chessBoard.GetPieceAt(x, y1) == nullptr))
					{
						outResult.emplace_back(x, y1);

						if (chessPiece->IsFirstMove())
						{
							int y2 = y + offset * 2;
							if ((0 <= y2 && y2 < height) && (chessBoard.GetPieceAt(x, y2) == nullptr))
							{
								outResult.emplace_back(x, y2);
							}
						}
					}

					// Pawn capture
					int x1 = x - 1;
					if ((0 <= x1 && x1 < width) 
						&& (chessBoard.GetPieceAt(x1, y1) != nullptr)
						&& (chessBoard.GetPieceAt(x1, y1)->GetPlayerIndex() != chessPiece->GetPlayerIndex()))
					{
						outResult.emplace_back(x1, y1);
					}
					int x2 = x + 1;
					if ((0 <= x2 && x2 < width)
						&& (chessBoard.GetPieceAt(x2, y1) != nullptr)
						&& (chessBoard.GetPieceAt(x2, y1)->GetPlayerIndex() != chessPiece->GetPlayerIndex()))
					{
						outResult.emplace_back(x2, y1);
					}
				}
				if (chessPiece->GetType() == ChessPiece::Type::Knight)
				{
					int dx[] = { -2, -2, -1, -1, 1,  1, 2,  2 };
					int dy[] = {  1, -1,  2, -2, 2, -2, 1, -1 };
					for (size_t i = 0; i < 8; ++i)
					{
						int xTemp = x + dx[i];
						int yTemp = y + dy[i];

						if ((0 <= xTemp && xTemp < width) && (0 <= yTemp && yTemp < height))
						{
							ChessPiece* targetPiece = chessBoard.GetPieceAt(xTemp, yTemp);
							if (targetPiece == nullptr || targetPiece->GetPlayerIndex() != chessPiece->GetPlayerIndex())
							{
								outResult.emplace_back(xTemp, yTemp);
							}
						}
					}
				}
				if (chessPiece->GetType() == ChessPiece::Type::Bishop || chessPiece->GetType() == ChessPiece::Type::Queen)
				{
					for (int dx = -1; dx <= 1; dx += 2)
					{
						for (int dy = -1; dy <= 1; dy += 2)
						{
							int xTemp = x;
							int yTemp = y;
							while (true)
							{
								xTemp += dx;
								yTemp += dy;
								ChessPiece* targetPiece = chessBoard.GetPieceAt(xTemp, yTemp);
								if ((xTemp < 0 || xTemp >= width) 
									|| (yTemp < 0 || yTemp >= height))
								{
									break;
								}
								if (targetPiece == nullptr)
								{
									outResult.emplace_back(xTemp, yTemp);
								}
								else if (targetPiece->GetPlayerIndex() != chessPiece->GetPlayerIndex())
								{
									outResult.emplace_back(xTemp, yTemp);
									break;
								}
								else
								{
									break;
								}
							}
						}
					}
				}
				if (chessPiece->GetType() == ChessPiece::Type::Rook || chessPiece->GetType() == ChessPiece::Type::Queen)
				{
					int dx[] = { -1, 1,  0, 0 };
					int dy[] = {  0, 0, -1, 1 };
					for (size_t i = 0; i < 4; ++i)
					{
						int xTemp = x;
						int yTemp = y;
						while (true)
						{
							xTemp += dx[i];
							yTemp += dy[i];
							ChessPiece* targetPiece = chessBoard.GetPieceAt(xTemp, yTemp);
							if ((xTemp < 0 || xTemp >= width)
								|| (yTemp < 0 || yTemp >= height))
							{
								break;
							}
							if (targetPiece == nullptr)
							{
								outResult.emplace_back(xTemp, yTemp);
							}
							else if (targetPiece->GetPlayerIndex() != chessPiece->GetPlayerIndex())
							{
								outResult.emplace_back(xTemp, yTemp);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				if (chessPiece->GetType() == ChessPiece::Type::King)
				{
					for (int dx = -1; dx <= 1; ++dx)
					{
						for (int dy = -1; dy <= 1; ++dy)
						{
							if (dx != 0 || dy != 0)
							{
								int xTemp = x + dx;
								int yTemp = y + dy;
								ChessPiece* targetPiece = chessBoard.GetPieceAt(xTemp, yTemp);
								if ((0 <= xTemp && xTemp < width) && (0 <= yTemp && yTemp < height))
								{
									ChessPiece* targetPiece = chessBoard.GetPieceAt(xTemp, yTemp);
									if (targetPiece == nullptr || targetPiece->GetPlayerIndex() != chessPiece->GetPlayerIndex())
									{
										outResult.emplace_back(xTemp, yTemp);
									}
								}
							}
						}
					}
				}
			}

			int GetWinner(const ChessBoard& chessBoard) override
			{
				return -1; // Draw
			}
		};
	}
}