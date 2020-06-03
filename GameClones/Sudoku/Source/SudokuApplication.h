#pragma once

#include "Common/BaseApplication.h"

namespace GameClones
{
	class SudokuApplication : public Common::BaseApplication
	{
	public:
		SudokuApplication();
		~SudokuApplication();

	protected:
		void SetupWindow(Common::Window* window) override;

		void Init() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void Cleanup() override;
	};
}