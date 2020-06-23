#include "ChessApplication.h"

namespace GameClones
{
	namespace Chess
	{
		ChessApplication::ChessApplication()
			: Common::BaseApplication("Chess")
		{
		}

		ChessApplication::~ChessApplication()
		{
		}

		void ChessApplication::SetupWindow(Common::Window* window)
		{
			window->SetSize(800, 600);
			window->SetTitle("Chess");
		}

		void ChessApplication::Init()
		{
		}

		void ChessApplication::Update(float deltaTime)
		{
		}

		void ChessApplication::Draw()
		{
		}

		void ChessApplication::Cleanup()
		{
		}
	}
}
