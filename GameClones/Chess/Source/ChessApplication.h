#pragma once

#include <unordered_map>
#include <vector>

#include "Common/BaseApplication.h"
#include "Common/RectangleShape.h"
#include "Common/RenderTarget.h"
#include "Common/Sprite.h"
#include "Common/Vector.h"

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "ChessRuleSet.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessApplication : public Common::BaseApplication
		{
		private:
			// Vector represents the player indices (m_pieceSprites[0] is the sprite map for player 1, etc...)
			std::vector<std::unordered_map<ChessPiece::Type, Common::Sprite*>> m_pieceSprites;
			Common::RenderTarget m_renderTarget;

			Common::RectangleShape m_rectangleShape;
			ChessBoard m_chessBoard;

			ChessPiece* m_draggedPiece;
			int prevCellX, prevCellY; // Previous cell position of a dragged piece

			std::vector<Common::Vector2> m_possibleCells;
			ChessRuleSet* m_ruleSet;

		public:
			ChessApplication();
			~ChessApplication();

		protected:
			void SetupWindow(Common::Window* window) override;

			void Init() override;
			void Update(float deltaTime) override;
			void Draw() override;
			void Cleanup() override;

		private:
			void ResetBoard();
		};
	}
}