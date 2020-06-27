#include "ChessApplication.h"

#include "Common/Color.h"

namespace GameClones
{
	namespace Chess
	{
		ChessApplication::ChessApplication()
			: Common::BaseApplication("Chess")
			, m_renderTarget()
			, m_rectangleShape()
			, m_chessBoard(8, 8)
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
		}

		void ChessApplication::Update(float deltaTime)
		{
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
						m_rectangleShape.SetFillColor(Common::Color::Green);
					}
					else
					{
						m_rectangleShape.SetFillColor(Common::Color::White);
					}

					m_renderTarget.Draw(m_rectangleShape);

					isDarkColoredSquare = !isDarkColoredSquare;
				}
			}
		}

		void ChessApplication::Cleanup()
		{
		}
	}
}
