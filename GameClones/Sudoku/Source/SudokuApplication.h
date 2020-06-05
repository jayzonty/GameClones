#pragma once

#include "Common/BaseApplication.h"
#include "Common/RenderTarget.h"

#include "SudokuGrid.h"
#include "SudokuGridChecker.h"
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
			SudokuGridChecker m_sudokuGridChecker;
			SudokuGridRenderer m_sudokuGridRenderer;

			size_t m_selectedRow;
			size_t m_selectedColumn;

		public:
			SudokuApplication();
			~SudokuApplication();

		protected:
			void SetupWindow(Common::Window* window) override;

			void Init() override;
			void Update(float deltaTime) override;
			void Draw() override;
			void Cleanup() override;

		private:
			float GetCellSize() const;
		};
	}
}