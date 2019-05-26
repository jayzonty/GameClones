#include "TetrisGame.h"

#include "Common/Window.h"
#include "Common/WindowManager.h"

#include "Common/Color.h"
#include "Common/Input.h"

#include "Common/Random.h"

namespace GameClones {
	TetrisGame::TetrisGame() : Common::BaseApplication("Tetris") {

	}

	TetrisGame::~TetrisGame() {

	}

	void TetrisGame::SetupWindow(Common::Window* window) {
		window->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		window->SetTitle("Tetris");
	}

	void TetrisGame::Init() {
		m_lines.resize(GRID_NUM_ROWS);
		for (auto& line : m_lines) {
			line.contents.resize(GRID_NUM_COLUMNS, -1);
		}

		m_gridBounds.x = 20.0f;
		m_gridBounds.y = 10.0f;
		m_gridBounds.width = 300.0f;
		m_gridBounds.height = 600.0f;
		
		m_renderTarget.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

		m_cellSize = 30.0f;

		InitTetriminoes();

		m_tetriminoIndex = 0;
		m_blockX = 0;
		m_blockY = 0;
		m_orientation = 0;

		m_dropSpeed = 2.0f;

		m_dropTimer.SetDuration(1.0f / m_dropSpeed);
		m_dropTimer.Start();
		SpawnTetrimino();
	}

	void TetrisGame::Update(float deltaTime) {
		m_dropTimer.Update(deltaTime);
		if (m_dropTimer.IsFinished()) {
			if (CanPieceFall(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY)) {
				--m_blockY;
			}
			else {
				Tetris::Tetrimino& tetrimino = m_tetriminoes[m_tetriminoIndex];
				Tetris::Block& block = tetrimino.blocks[m_orientation];
				
				for (int y = 0; y < block.height; ++y) {
					Line& line = GetLine(m_blockY + y);
					for (int x = 0; x < block.width; ++x) {
						if (block.grid[x][y]) {
							line.contents[m_blockX + x] = m_tetriminoIndex;
						}
					}
				}

				SpawnTetrimino();
			}
			
			m_dropTimer.Start();
		}

		Tetris::Tetrimino& tetrimino = m_tetriminoes[m_tetriminoIndex];
		Tetris::Block& block = tetrimino.blocks[m_orientation];
		if (Common::Input::IsPressed(Common::Input::UP)) {
			m_orientation = (m_orientation + 1) % 4;
		}
		else if (Common::Input::IsPressed(Common::Input::LEFT)) {
			if (CanPieceMoveLeft(tetrimino, m_orientation, m_blockX, m_blockY)) {
				--m_blockX;
			}
		}
		else if (Common::Input::IsPressed(Common::Input::RIGHT)) {
			if (CanPieceMoveRight(tetrimino, m_orientation, m_blockX, m_blockY)) {
				++m_blockX;
			}
		}
	}

	void TetrisGame::Draw() {
		// Draw playing field border
		m_shape.SetPosition(m_gridBounds.x, m_gridBounds.y);
		m_shape.SetSize(m_gridBounds.width + 10.0f, m_gridBounds.height);
		m_shape.SetBorderColor(Common::Color::White);
		m_shape.SetFillColor(Common::Color::Clear);
		m_shape.SetBorderSizes(5.0f, 5.0f, 0.0f, 5.0f);
		m_renderTarget.Draw(m_shape);

		for (size_t y = 0; y < m_lines.size(); ++y) {
			Line& line = GetLine(y);
			for (size_t x = 0; x < line.contents.size(); ++x) {
				if (line.contents[x] != -1) {
					float xPos = m_gridBounds.x + 5.0f + x * m_cellSize;
					float yPos = m_gridBounds.y + 5.0f + y * m_cellSize;

					m_shape.SetPosition(xPos, yPos);
					m_shape.SetSize(m_cellSize, m_cellSize);
					m_shape.SetFillColor(m_tetriminoes[line.contents[x]].color);
					m_shape.SetBorderSizes(2.0f, 2.0f, 2.0f, 2.0f);
					m_renderTarget.Draw(m_shape);
				}
			}
		}

		// Draw current tetrimino block we're controlling
		DrawTetrimino(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY);;
	}

	void TetrisGame::Cleanup() {
	}

	void TetrisGame::InitTetriminoes() {
		m_tetriminoes.push_back(CreateLTetrimino());
		m_tetriminoes.push_back(CreateReverseLTetrimino());
		m_tetriminoes.push_back(CreateTTetrimino());
		m_tetriminoes.push_back(CreateSquareTetrimino());
		m_tetriminoes.push_back(CreateSTetrimino());
		m_tetriminoes.push_back(CreateReverseSTetrimino());
		m_tetriminoes.push_back(CreateStickTetrimino());
	}

	Tetris::Tetrimino TetrisGame::CreateLTetrimino() {
		// Left is column (x), right is row (y)
		GameClones::Tetris::Tetrimino lTetrimino;
		lTetrimino.color = Common::Color::Blue;

		// []
		// []
		// [][]
		lTetrimino.blocks[0].width = 2;
		lTetrimino.blocks[0].height = 3;
		lTetrimino.blocks[0].grid[0][0] = true;
		lTetrimino.blocks[0].grid[1][0] = true;
		lTetrimino.blocks[0].grid[0][1] = true;
		lTetrimino.blocks[0].grid[0][2] = true;

		// [][][]
		// []
		lTetrimino.blocks[1].width = 3;
		lTetrimino.blocks[1].height = 2;
		lTetrimino.blocks[1].grid[0][0] = true;
		lTetrimino.blocks[1].grid[0][1] = true;
		lTetrimino.blocks[1].grid[1][1] = true;
		lTetrimino.blocks[1].grid[2][1] = true;

		// [][]
		//   []
		//   []
		lTetrimino.blocks[2].width = 2;
		lTetrimino.blocks[2].height = 3;
		lTetrimino.blocks[2].grid[1][0] = true;
		lTetrimino.blocks[2].grid[1][1] = true;
		lTetrimino.blocks[2].grid[1][2] = true;
		lTetrimino.blocks[2].grid[0][2] = true;

		//     []
		// [][][]
		lTetrimino.blocks[3].width = 3;
		lTetrimino.blocks[3].height = 2;
		lTetrimino.blocks[3].grid[0][0] = true;
		lTetrimino.blocks[3].grid[1][0] = true;
		lTetrimino.blocks[3].grid[2][0] = true;
		lTetrimino.blocks[3].grid[2][1] = true;

		return lTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateReverseLTetrimino() {
		GameClones::Tetris::Tetrimino rlTetrimino;
		rlTetrimino.color = Common::Color::Magenta;

		//   []
		//   []
		// [][]
		rlTetrimino.blocks[0].width = 2;
		rlTetrimino.blocks[0].height = 3;
		rlTetrimino.blocks[0].grid[0][0] = true;
		rlTetrimino.blocks[0].grid[1][0] = true;
		rlTetrimino.blocks[0].grid[1][1] = true;
		rlTetrimino.blocks[0].grid[1][2] = true;

		// []
		// [][][]
		rlTetrimino.blocks[1].width = 3;
		rlTetrimino.blocks[1].height = 2;
		rlTetrimino.blocks[1].grid[0][0] = true;
		rlTetrimino.blocks[1].grid[0][1] = true;
		rlTetrimino.blocks[1].grid[1][0] = true;
		rlTetrimino.blocks[1].grid[2][0] = true;

		// [][]
		// []
		// []
		rlTetrimino.blocks[2].width = 2;
		rlTetrimino.blocks[2].height = 3;
		rlTetrimino.blocks[2].grid[0][0] = true;
		rlTetrimino.blocks[2].grid[0][1] = true;
		rlTetrimino.blocks[2].grid[0][2] = true;
		rlTetrimino.blocks[2].grid[1][2] = true;

		// [][][]
		//     []
		rlTetrimino.blocks[3].width = 3;
		rlTetrimino.blocks[3].height = 2;
		rlTetrimino.blocks[3].grid[0][1] = true;
		rlTetrimino.blocks[3].grid[1][1] = true;
		rlTetrimino.blocks[3].grid[2][1] = true;
		rlTetrimino.blocks[3].grid[2][0] = true;

		return rlTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateTTetrimino() {
		Tetris::Tetrimino tTetrimino;
		tTetrimino.color = Common::Color::Red;

		//   []
		// [][][]
		tTetrimino.blocks[0].width = 3;
		tTetrimino.blocks[0].height = 2;
		tTetrimino.blocks[0].grid[0][0] = true;
		tTetrimino.blocks[0].grid[1][0] = true;
		tTetrimino.blocks[0].grid[2][0] = true;
		tTetrimino.blocks[0].grid[1][1] = true;

		// []
		// [][]
		// []
		tTetrimino.blocks[1].width = 2;
		tTetrimino.blocks[1].height = 3;
		tTetrimino.blocks[1].grid[0][0] = true;
		tTetrimino.blocks[1].grid[0][1] = true;
		tTetrimino.blocks[1].grid[1][1] = true;
		tTetrimino.blocks[1].grid[0][2] = true;

		// [][][]
		//   []
		tTetrimino.blocks[2].width = 3;
		tTetrimino.blocks[2].height = 2;
		tTetrimino.blocks[2].grid[0][1] = true;
		tTetrimino.blocks[2].grid[1][1] = true;
		tTetrimino.blocks[2].grid[1][0] = true;
		tTetrimino.blocks[2].grid[2][1] = true;

		//   []
		// [][]
		//   []
		tTetrimino.blocks[3].width = 2;
		tTetrimino.blocks[3].height = 3;
		tTetrimino.blocks[3].grid[1][0] = true;
		tTetrimino.blocks[3].grid[1][1] = true;
		tTetrimino.blocks[3].grid[0][1] = true;
		tTetrimino.blocks[3].grid[1][2] = true;

		return tTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateSquareTetrimino() {
		Tetris::Tetrimino squareTetrimino;
		squareTetrimino.color = Common::Color::Green;

		// [][]
		// [][]
		for (size_t i = 0; i < 4; ++i) {
			squareTetrimino.blocks[i].width = 2;
			squareTetrimino.blocks[i].height = 2;
			squareTetrimino.blocks[i].grid[0][0] = true;
			squareTetrimino.blocks[i].grid[0][1] = true;
			squareTetrimino.blocks[i].grid[1][0] = true;
			squareTetrimino.blocks[i].grid[1][1] = true;
		}

		return squareTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateSTetrimino() {
		Tetris::Tetrimino sTetrimino;
		sTetrimino.color = Common::Color::Orange;

		for (size_t i = 0; i < 4; ++i) {
			if (i % 2 == 0) {
				//   [][]
				// [][]
				sTetrimino.blocks[i].width = 3;
				sTetrimino.blocks[i].height = 2;
				sTetrimino.blocks[i].grid[0][0] = true;
				sTetrimino.blocks[i].grid[1][0] = true;
				sTetrimino.blocks[i].grid[1][1] = true;
				sTetrimino.blocks[i].grid[2][1] = true;
			}
			else {
				// []
				// [][]
				//   []
				sTetrimino.blocks[i].width = 2;
				sTetrimino.blocks[i].height = 3;
				sTetrimino.blocks[i].grid[1][0] = true;
				sTetrimino.blocks[i].grid[1][1] = true;
				sTetrimino.blocks[i].grid[0][1] = true;
				sTetrimino.blocks[i].grid[0][2] = true;
			}
		}

		return sTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateReverseSTetrimino() {
		Tetris::Tetrimino rsTetrimino;
		rsTetrimino.color = Common::Color::Pink;

		for (size_t i = 0; i < 4; ++i) {
			if (i % 2 == 0) {
				// [][]
				//   [][]
				rsTetrimino.blocks[i].width = 3;
				rsTetrimino.blocks[i].height = 2;
				rsTetrimino.blocks[i].grid[0][1] = true;
				rsTetrimino.blocks[i].grid[1][1] = true;
				rsTetrimino.blocks[i].grid[1][0] = true;
				rsTetrimino.blocks[i].grid[2][0] = true;
			}
			else {
				//   []
				// [][]
				// []
				rsTetrimino.blocks[i].width = 2;
				rsTetrimino.blocks[i].height = 3;
				rsTetrimino.blocks[i].grid[0][0] = true;
				rsTetrimino.blocks[i].grid[0][1] = true;
				rsTetrimino.blocks[i].grid[1][1] = true;
				rsTetrimino.blocks[i].grid[1][2] = true;
			}
		}

		return rsTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateStickTetrimino() {
		Tetris::Tetrimino stickTetrimino;
		stickTetrimino.color = Common::Color::Cyan;

		// []
		// []
		// []
		// []
		stickTetrimino.blocks[0].width = 1;
		stickTetrimino.blocks[0].height = 4;
		stickTetrimino.blocks[0].grid[0][0] = true;
		stickTetrimino.blocks[0].grid[0][1] = true;
		stickTetrimino.blocks[0].grid[0][2] = true;
		stickTetrimino.blocks[0].grid[0][3] = true;

		// [][][][]
		stickTetrimino.blocks[1].width = 4;
		stickTetrimino.blocks[1].height = 1;
		stickTetrimino.blocks[1].grid[0][0] = true;
		stickTetrimino.blocks[1].grid[1][0] = true;
		stickTetrimino.blocks[1].grid[2][0] = true;
		stickTetrimino.blocks[1].grid[3][0] = true;

		// []
		// []
		// []
		// []
		stickTetrimino.blocks[2].width = 1;
		stickTetrimino.blocks[2].height = 4;
		stickTetrimino.blocks[2].grid[0][0] = true;
		stickTetrimino.blocks[2].grid[0][1] = true;
		stickTetrimino.blocks[2].grid[0][2] = true;
		stickTetrimino.blocks[2].grid[0][3] = true;

		// [][][][]
		stickTetrimino.blocks[3].width = 4;
		stickTetrimino.blocks[3].height = 1;
		stickTetrimino.blocks[3].grid[0][0] = true;
		stickTetrimino.blocks[3].grid[1][0] = true;
		stickTetrimino.blocks[3].grid[2][0] = true;
		stickTetrimino.blocks[3].grid[3][0] = true;

		return stickTetrimino;
	}

	void TetrisGame::DrawTetrimino(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		const Tetris::Block& block = tetrimino.blocks[orientation];

		float borderSize = 2.0f;

		m_shape.SetBorderColor(Common::Color::White);
		m_shape.SetFillColor(tetrimino.color);

		for (size_t x = 0; x < block.width; ++x) {
			for (size_t y = 0; y < block.height; ++y) {
				float xPos = m_gridBounds.x + 5.0f + (cellX + x) * m_cellSize;
				float yPos = m_gridBounds.y + 5.0f + (cellY + y) * m_cellSize;
				float width = m_cellSize;
				float height = m_cellSize;

				if (!tetrimino.blocks[orientation].grid[x][y]) {
					continue;
				}

				m_shape.SetBorderSizes(borderSize, borderSize, borderSize, borderSize);
				m_shape.SetPosition(xPos, yPos);
				m_shape.SetSize(width, height);
				m_renderTarget.Draw(m_shape);
			}
		}
	}

	bool TetrisGame::CanPieceFall(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		if (cellY == 0) {
			return false;
		}

		const Tetris::Block& block = tetrimino.blocks[orientation];
		for (size_t y = 0; y < block.height; ++y) {
			Line& lineBelow = GetLine(cellY + y - 1);
			for (size_t x = 0; x < block.width; ++x) {
				if (block.grid[x][y] && lineBelow.contents[cellX + x] != -1) {
					return false;
				}
			}
		}

		return true;
	}

	bool TetrisGame::CanPieceMoveLeft(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		if (cellX == 0) {
			return false;
		}

		const Tetris::Block& block = tetrimino.blocks[orientation];
		for (size_t y = 0; y < block.height; ++y) {
			Line& line = GetLine(cellY + y);
			for (size_t x = 0; x < block.width; ++x) {
				if (block.grid[x][y] && line.contents[cellX + x - 1] != -1) {
					return false;
				}
			}
		}

		return true;
	}

	bool TetrisGame::CanPieceMoveRight(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		const Tetris::Block& block = tetrimino.blocks[orientation];
		if (cellX + block.width >= GRID_NUM_COLUMNS) {
			return false;
		}

		
		for (size_t y = 0; y < block.height; ++y) {
			Line& line = GetLine(cellY + y);
			for (size_t x = 0; x < block.width; ++x) {
				if (block.grid[x][y] && line.contents[cellX + x + 1] != -1) {
					return false;
				}
			}
		}

		return true;
	}

	bool TetrisGame::CanRotate(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		const Tetris::Block& block = tetrimino.blocks[(orientation + 1) % 4];
		return false;
	}

	bool TetrisGame::IsPieceColliding(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		return false;
	}

	void TetrisGame::SpawnTetrimino() {
		Common::Random random;
		m_tetriminoIndex = random(0, m_tetriminoes.size());

		m_orientation = 0;
		m_blockX = (GRID_NUM_COLUMNS - m_tetriminoes[m_tetriminoIndex].blocks[m_orientation].width) / 2;
		m_blockY = 17;
	}

	TetrisGame::Line& TetrisGame::GetLine(int lineIndex) {
		auto it = m_lines.begin();
		while (lineIndex-- > 0) {
			++it;
		}

		return *it;
	}
}