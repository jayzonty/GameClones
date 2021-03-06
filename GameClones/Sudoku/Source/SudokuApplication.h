#pragma once

#include <string>
#include <vector>

#include "Common/BaseApplication.h"
#include "Common/Button.h"
#include "Common/Font.h"
#include "Common/Random.h"
#include "Common/RenderTarget.h"
#include "Common/Text.h"

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
			Common::Random m_random;
			Common::RenderTarget m_renderTarget;

			Common::Font m_buttonsFont;
			Common::Button m_newGameButton;
			Common::Button m_checkSolutionButton;
			Common::Text m_resultText;

			SudokuGrid m_sudokuGrid;
			SudokuGridChecker m_sudokuGridChecker;
			SudokuGridRenderer m_sudokuGridRenderer;

			size_t m_selectedRow;
			size_t m_selectedColumn;

			std::vector<std::string> m_puzzleStrings;

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
			void NewGame();
			void CheckSolution();

			float GetCellSize() const;

			void ReadPuzzleStrings(const std::string& puzzlesFile, std::vector<std::string>& puzzleStrings);
			void LoadGridFromPuzzleString(const std::string& puzzleString, SudokuGrid& outGrid);
		};
	}
}