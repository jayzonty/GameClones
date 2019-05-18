#pragma once

#ifndef ___GameClones_SlidingPuzzle_SlidingPuzzle_h___
#define ___GameClones_SlidingPuzzle_SlidingPuzzle_h___

#include "Common/BaseApplication.h"
#include "Common/RenderTarget.h"
#include "Common/RectangleShape.h"
#include "Common/Text.h"
#include "Common/Rect.h"
#include "Common/Button.h"
#include "Common/Font.h"

#include "Panel.h"

namespace GameClones {
	namespace SlidingPuzzle {
		class SlidingPuzzle : public Common::BaseApplication {
			const unsigned int WINDOW_WIDTH = 800;
			const unsigned int WINDOW_HEIGHT = 800;

			enum class State {
				Start, Play, Win
			};
			State m_currentState;

			Common::Rect<int> m_gridBounds;
			
			Common::RenderTarget m_renderTarget;
			Common::RectangleShape m_backgroundShape;

			unsigned int m_rows, m_columns;

			unsigned int m_blankRow, m_blankColumn;
			unsigned int m_blankRowCopy, m_blankColumnCopy;

			std::vector<Panel*> m_panelGrid;
			std::vector<Panel*> m_panelGridCopy;

			Common::Text m_statusText;

			Common::Button m_newGameButton;
			Common::Button m_resetButton;

			Common::Font m_textFont;

		public:
			SlidingPuzzle();
			~SlidingPuzzle();

			void Init() override;
			void Update(float deltaTime) override;
			void Draw() override;
			void Cleanup() override;

		private:
			void Shuffle();
			void ResetGrid();

			void CheckWin();

			int GetIndex(int row, int column);

			void SwapPanels(int panel1Row, int panel1Column, int panel2Row, int panel2Column);

			bool CanPressUp();
			bool CanPressDown();
			bool CanPressLeft();
			bool CanPressRight();

			bool PressUp();
			bool PressDown();
			bool PressLeft();
			bool PressRight();

			void GetBlankCellPosition(const std::vector<Panel*>& grid, unsigned int& row, unsigned int& column);
		};
	}
}

#endif // ___GameClones_SlidingPuzzle_SlidingPuzzle_h___
