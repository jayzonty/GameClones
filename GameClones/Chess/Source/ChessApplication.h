#pragma once

#include "Common/BaseApplication.h"
#include "Common/RectangleShape.h"
#include "Common/RenderTarget.h"

#include "ChessBoard.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessApplication : public Common::BaseApplication
		{
		private:
			Common::RenderTarget m_renderTarget;

			Common::RectangleShape m_rectangleShape;
			ChessBoard m_chessBoard;

		public:
			ChessApplication();
			~ChessApplication();

		protected:
			void SetupWindow(Common::Window* window) override;

			void Init() override;
			void Update(float deltaTime) override;
			void Draw() override;
			void Cleanup() override;
		};
	}
}