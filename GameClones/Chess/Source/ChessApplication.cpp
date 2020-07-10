#include "ChessApplication.h"

#include <iostream>

#include "Common/Color.h"
#include "Common/Input.h"
#include "Common/Texture.h"

#include "ChessStandardRuleSet.h"

namespace GameClones
{
	namespace Chess
	{
		ChessApplication::ChessApplication()
			: Common::BaseApplication("Chess")
			, m_renderTarget()
			, m_rectangleShape()
			, m_chessBoard(8, 8)
			, m_draggedPiece(nullptr)
			, m_possibleCells()
			, m_ruleSet(nullptr)
		{
		}

		ChessApplication::~ChessApplication()
		{
		}

		void ChessApplication::SetupWindow(Common::Window* window)
		{
			window->SetSize(1280, 720);
			window->SetTitle("Chess");

			m_renderTarget.SetSize(1280, 720);
		}

		void ChessApplication::Init()
		{
			m_rectangleShape.SetSize(90.0f, 90.0f);

			// White pieces
			m_pieceSprites.emplace_back();

			Common::Texture* whitePawnTexture = new Common::Texture();
			whitePawnTexture->Load("Resources/Textures/pawn_white.png");
			Common::Sprite* whitePawnSprite = new Common::Sprite(whitePawnTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Pawn, whitePawnSprite });

			Common::Texture* whiteRookTexture = new Common::Texture();
			whiteRookTexture->Load("Resources/Textures/rook_white.png");
			Common::Sprite* whiteRookSprite = new Common::Sprite(whiteRookTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Rook, whiteRookSprite });

			Common::Texture* whiteKnightTexture = new Common::Texture();
			whiteKnightTexture->Load("Resources/Textures/knight_white.png");
			Common::Sprite* whiteKnightSprite = new Common::Sprite(whiteKnightTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Knight, whiteKnightSprite });

			Common::Texture* whiteBishopTexture = new Common::Texture();
			whiteBishopTexture->Load("Resources/Textures/bishop_white.png");
			Common::Sprite* whiteBishopSprite = new Common::Sprite(whiteBishopTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Bishop, whiteBishopSprite });

			Common::Texture* whiteQueenTexture = new Common::Texture();
			whiteQueenTexture->Load("Resources/Textures/queen_white.png");
			Common::Sprite* whiteQueenSprite = new Common::Sprite(whiteQueenTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Queen, whiteQueenSprite });

			Common::Texture* whiteKingTexture = new Common::Texture();
			whiteKingTexture->Load("Resources/Textures/king_white.png");
			Common::Sprite* whiteKingSprite = new Common::Sprite(whiteKingTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::King, whiteKingSprite });

			// Black pieces
			m_pieceSprites.emplace_back();

			Common::Texture* blackPawnTexture = new Common::Texture();
			blackPawnTexture->Load("Resources/Textures/pawn_black.png");
			Common::Sprite* blackPawnSprite = new Common::Sprite(blackPawnTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Pawn, blackPawnSprite });

			Common::Texture* blackRookTexture = new Common::Texture();
			blackRookTexture->Load("Resources/Textures/rook_black.png");
			Common::Sprite* blackRookSprite = new Common::Sprite(blackRookTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Rook, blackRookSprite });

			Common::Texture* blackKnightTexture = new Common::Texture();
			blackKnightTexture->Load("Resources/Textures/knight_black.png");
			Common::Sprite* blackKnightSprite = new Common::Sprite(blackKnightTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Knight, blackKnightSprite });

			Common::Texture* blackBishopTexture = new Common::Texture();
			blackBishopTexture->Load("Resources/Textures/bishop_black.png");
			Common::Sprite* blackBishopSprite = new Common::Sprite(blackBishopTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Bishop, blackBishopSprite });

			Common::Texture* blackQueenTexture = new Common::Texture();
			blackQueenTexture->Load("Resources/Textures/queen_black.png");
			Common::Sprite* blackQueenSprite = new Common::Sprite(blackQueenTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::Queen, blackQueenSprite });

			Common::Texture* blackKingTexture = new Common::Texture();
			blackKingTexture->Load("Resources/Textures/king_black.png");
			Common::Sprite* blackKingSprite = new Common::Sprite(blackKingTexture);
			m_pieceSprites.back().insert({ ChessPiece::Type::King, blackKingSprite });

			ResetBoard();

			m_ruleSet = new ChessStandardRuleSet();
		}

		void ChessApplication::Update(float deltaTime)
		{
			int mouseX = 0, mouseY = 0;
			int cellX = -1, cellY = -1;
			Common::Input::GetMousePosition(&mouseX, &mouseY);
			mouseY = 720 - mouseY;
			cellX = static_cast<int>(mouseX / 90.0f);
			cellY = static_cast<int>(mouseY / 90.0f);

			if (m_draggedPiece == nullptr)
			{
				if (Common::Input::IsPressed(Common::Input::LEFT_MOUSE))
				{
					m_draggedPiece = m_chessBoard.GetPieceAt(cellX, cellY);
					if (m_draggedPiece != nullptr)
					{
						prevCellX = cellX;
						prevCellY = cellY;

						m_ruleSet->GetPossibleCells(m_draggedPiece, Common::Vector2(cellX, cellY), m_chessBoard, m_possibleCells);
					}
				}
			}
			else
			{
				if (Common::Input::IsReleased(Common::Input::LEFT_MOUSE))
				{
					if (m_draggedPiece != nullptr)
					{
						for (size_t i = 0; i < m_possibleCells.size(); ++i)
						{
							if ((m_possibleCells[i].x == cellX) && (m_possibleCells[i].y == cellY))
							{
								m_chessBoard.SetPieceAt(prevCellX, prevCellY, nullptr);
								m_chessBoard.SetPieceAt(cellX, cellY, m_draggedPiece);
								m_draggedPiece->SetIsFirstMove(false);
								break;
							}
						}
						
						m_draggedPiece = nullptr;
						m_possibleCells.clear();
					}
				}
			}
		}

		void ChessApplication::Draw()
		{
			size_t boardWidth = 0, boardHeight = 0;
			m_chessBoard.GetSize(boardWidth, boardHeight);

			for (size_t y = 0; y < boardHeight; ++y)
			{
				bool isDarkColoredSquare = (y % 2) == 0;

				for (size_t x = 0; x < boardWidth; ++x)
				{
					m_rectangleShape.SetPosition(x * 90.0f, y * 90.0f);

					if (isDarkColoredSquare)
					{
						m_rectangleShape.SetFillColor(Common::Color(81, 42, 42, 255));
					}
					else
					{
						m_rectangleShape.SetFillColor(Common::Color(124, 76, 62, 255));
					}

					m_renderTarget.Draw(m_rectangleShape);

					isDarkColoredSquare = !isDarkColoredSquare;
				}
			}

			for (size_t y = 0; y < boardHeight; ++y)
			{
				for (size_t x = 0; x < boardWidth; ++x)
				{
					// Draw piece
					ChessPiece* chessPiece = m_chessBoard.GetPieceAt(x, y);
					if (chessPiece != nullptr)
					{
						Common::Sprite* sprite = m_pieceSprites[chessPiece->GetPlayerIndex()][chessPiece->GetType()];
						if (sprite != nullptr)
						{
							if (chessPiece == m_draggedPiece)
							{
								int mouseX = 0, mouseY = 0;
								Common::Input::GetMousePosition(&mouseX, &mouseY);
								mouseY = 720 - mouseY;

								sprite->SetPosition(mouseX - 30.0f, mouseY - 30.0f);
							}
							else
							{
								sprite->SetPosition(x * 90.0f + 15.0f, y * 90.0f + 10.0f);
							}
							
							sprite->SetSize(60.0f, 60.0f);
							
							m_renderTarget.Draw(*sprite);
						}
					}
				}
			}
		}

		void ChessApplication::Cleanup()
		{
			for (size_t i = 0; i < m_pieceSprites.size(); ++i)
			{
				for (auto it = m_pieceSprites[i].begin(); it != m_pieceSprites[i].end(); ++it)
				{
					delete it->second->GetTexture();
					delete it->second;
				}
				m_pieceSprites[i].clear();
			}
			m_pieceSprites.clear();

			delete m_ruleSet;
			m_ruleSet = nullptr;
		}

		void ChessApplication::ResetBoard()
		{
			// For now, use the standard chess rules to populate the board

			// --- White pieces ---
			m_chessBoard.SetPieceAt(0, 0, new ChessPiece(ChessPiece::Type::Rook, 0));
			m_chessBoard.SetPieceAt(1, 0, new ChessPiece(ChessPiece::Type::Knight, 0));
			m_chessBoard.SetPieceAt(2, 0, new ChessPiece(ChessPiece::Type::Bishop, 0));
			m_chessBoard.SetPieceAt(3, 0, new ChessPiece(ChessPiece::Type::Queen, 0));
			m_chessBoard.SetPieceAt(4, 0, new ChessPiece(ChessPiece::Type::King, 0));
			m_chessBoard.SetPieceAt(5, 0, new ChessPiece(ChessPiece::Type::Bishop, 0));
			m_chessBoard.SetPieceAt(6, 0, new ChessPiece(ChessPiece::Type::Knight, 0));
			m_chessBoard.SetPieceAt(7, 0, new ChessPiece(ChessPiece::Type::Rook, 0));
			for (size_t i = 0; i < 8; ++i)
			{
				m_chessBoard.SetPieceAt(i, 1, new ChessPiece(ChessPiece::Type::Pawn, 0));
			}

			// --- Black pieces ---
			m_chessBoard.SetPieceAt(0, 7, new ChessPiece(ChessPiece::Type::Rook, 1));
			m_chessBoard.SetPieceAt(1, 7, new ChessPiece(ChessPiece::Type::Knight, 1));
			m_chessBoard.SetPieceAt(2, 7, new ChessPiece(ChessPiece::Type::Bishop, 1));
			m_chessBoard.SetPieceAt(3, 7, new ChessPiece(ChessPiece::Type::Queen, 1));
			m_chessBoard.SetPieceAt(4, 7, new ChessPiece(ChessPiece::Type::King, 1));
			m_chessBoard.SetPieceAt(5, 7, new ChessPiece(ChessPiece::Type::Bishop, 1));
			m_chessBoard.SetPieceAt(6, 7, new ChessPiece(ChessPiece::Type::Knight, 1));
			m_chessBoard.SetPieceAt(7, 7, new ChessPiece(ChessPiece::Type::Rook, 1));
			for (size_t i = 0; i < 8; ++i)
			{
				m_chessBoard.SetPieceAt(i, 6, new ChessPiece(ChessPiece::Type::Pawn, 1));
			}
		}
	}
}
