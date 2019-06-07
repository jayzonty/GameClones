#pragma once

#ifndef ___GameClones_Tetris_h___
#define ___GameClones_Tetris_h___

#include "Common/BaseApplication.h"
#include "Common/Font.h"
#include "Common/RectangleShape.h"
#include "Common/RenderTarget.h"
#include "Common/Rect.h"
#include "Common/Text.h"
#include "Common/Timer.h"

#include "Common/Window.h"

#include "Tetrimino.h"

#include <list>
#include <vector>

namespace GameClones {
	class TetrisGame : public Common::BaseApplication {
	private:
		struct Line {
			std::vector<int> contents;

			bool IsFull() const {
				for (size_t i = 0; i < contents.size(); ++i) {
					if (contents[i] == -1) {
						return false;
					}
				}

				return true;
			}
		};

		const unsigned int WINDOW_WIDTH = 600;
		const unsigned int WINDOW_HEIGHT = 620;

		const unsigned int GRID_NUM_ROWS = 30;
		const unsigned int GRID_NUM_COLUMNS = 10;

		float m_cellSize;

		int m_tetriminoIndex;
		int m_nextTetriminoIndex;

		int m_blockX, m_blockY;
		int m_orientation;

		Common::RectangleShape m_shape;

		Common::RenderTarget m_renderTarget;

		Common::Rect<float> m_gridBounds;
		Common::Rect<float> m_nextBlockUIBounds;

		std::vector<GameClones::Tetris::Tetrimino> m_tetriminoes;

		Common::Timer m_dropTimer;
		float m_dropSpeed;

		std::list<Line> m_lines;

		Common::Font m_font;
		Common::Text m_text;

		bool m_isGameOver;

		int m_score;
		int m_totalLinesCleared;

		int m_level;
		int m_linesToNextLevel;


	public:
		TetrisGame();
		~TetrisGame();

	protected:
		void SetupWindow(Common::Window* window) override;

		void Init() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void Cleanup() override;

	private:
		void InitTetriminoes();

		Tetris::Tetrimino CreateLTetrimino();
		Tetris::Tetrimino CreateReverseLTetrimino();
		Tetris::Tetrimino CreateTTetrimino();
		Tetris::Tetrimino CreateSquareTetrimino();
		Tetris::Tetrimino CreateSTetrimino();
		Tetris::Tetrimino CreateReverseSTetrimino();
		Tetris::Tetrimino CreateStickTetrimino();

		void DrawTetrimino(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);
		void DrawTetrimino2(const Tetris::Tetrimino& tetrimino, int orientation, float originX, float originY);

		bool CanPieceMove(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY, int dirX, int dirY);
		void Rotate(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);
		bool IsPieceColliding(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);

		void SpawnTetrimino();

		Line& GetLine(int lineIndex);

		// Returns the number of lines cleared
		int ClearCompleteLines();

		int GetYOnDrop(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);

		void Reset();
	};
}

#endif // ___GameClones_Tetris_h___
