#pragma once

#include "Common/BaseApplication.h"
#include "Common/RenderTarget.h"

#include "SudokuGrid.h"
#include "SudokuGridRenderer.h"

namespace GameClones
{
	namespace Sudoku
	{
		class SudokuApplication : public Common::BaseApplication
		{
		private:
			Common::RenderTarget m_renderTarget;

			SudokuGrid m_sudokuGrid;
			SudokuGridRenderer m_sudokuGridRenderer;

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
}