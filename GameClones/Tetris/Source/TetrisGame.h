#pragma once

#ifndef ___GameClones_Tetris_h___
#define ___GameClones_Tetris_h___

#include "Common/BaseApplication.h"
#include "Common/RectangleShape.h"
#include "Common/RenderTarget.h"
#include "Common/Rect.h"
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
		};

		const unsigned int WINDOW_WIDTH = 600;
		const unsigned int WINDOW_HEIGHT = 620;

		const unsigned int GRID_NUM_ROWS = 30;
		const unsigned int GRID_NUM_COLUMNS = 10;

		float m_cellSize;

		int m_tetriminoIndex;
		int m_blockX, m_blockY;
		int m_orientation;

		Common::RectangleShape m_shape;

		Common::RenderTarget m_renderTarget;

		Common::Rect<float> m_gridBounds;

		std::vector<GameClones::Tetris::Tetrimino> m_tetriminoes;

		Common::Timer m_dropTimer;
		float m_dropSpeed;

		std::list<Line> m_lines;

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

		bool CanPieceFall(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);
		bool CanPieceMoveLeft(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);
		bool CanPieceMoveRight(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);
		bool CanRotate(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);

		bool IsPieceColliding(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY);

		void SpawnTetrimino();

		Line& GetLine(int lineIndex);
	};
}

#endif // ___GameClones_Tetris_h___
