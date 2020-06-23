#pragma once

#include "Common/BaseApplication.h"

namespace GameClones
{
	namespace Chess
	{
		class ChessApplication : public Common::BaseApplication
		{
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