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
		m_gridBounds.x = 20.0f;
		m_gridBounds.y = 10.0f;
		m_gridBounds.width = 300.0f;
		m_gridBounds.height = 605.0f;

		m_nextBlockUIBounds.x = 390.0f;
		m_nextBlockUIBounds.y = 10.0f;
		m_nextBlockUIBounds.width = 150.0f;
		m_nextBlockUIBounds.height = 150.0f;

		if (!m_font.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF")) {
			std::printf("Failed to load font!\n");
		}
		m_text.SetFont(m_font);

		m_isGameOver = false;
		
		m_renderTarget.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

		m_cellSize = 30.0f;

		InitTetriminoes();

		Reset();
		
		m_dropTimer.SetDuration(1.0f / m_dropSpeed);
		m_dropTimer.Start();
		SpawnTetrimino();
	}

	void TetrisGame::Update(float deltaTime) {
		if (m_isGameOver) {
			if (Common::Input::IsPressed(Common::Input::SPACE)) {
				Reset();

				m_dropTimer.SetDuration(1.0f / m_dropSpeed);
				m_dropTimer.Start();
				SpawnTetrimino();
			}

			return;
		}

		bool commitBlock = false;

		Tetris::Tetrimino& tetrimino = m_tetriminoes[m_tetriminoIndex];
		Tetris::Block& block = tetrimino.blocks[m_orientation];
		if (Common::Input::IsPressed(Common::Input::UP)) {
			Rotate(tetrimino, m_orientation, m_blockX, m_blockY);
		}
		else if (Common::Input::IsPressed(Common::Input::LEFT)) {
			if (CanPieceMove(tetrimino, m_orientation, m_blockX, m_blockY, -1, 0)) {
				--m_blockX;
			}
		}
		else if (Common::Input::IsPressed(Common::Input::RIGHT)) {
			if (CanPieceMove(tetrimino, m_orientation, m_blockX, m_blockY, 1, 0)) {
				++m_blockX;
			}
		}
		else if (Common::Input::IsPressed(Common::Input::DOWN)) {
			if (CanPieceMove(tetrimino, m_orientation, m_blockX, m_blockY, 0, -1)) {
				--m_blockY;
			}
			else {
				commitBlock = true;
			}
		}
		else if (Common::Input::IsPressed(Common::Input::SPACE)) {
			m_blockY = GetYOnDrop(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY);
			commitBlock = true;
		}

		m_dropTimer.Update(deltaTime);
		if (m_dropTimer.IsFinished()) {
			if (CanPieceMove(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY, 0, -1)) {
				--m_blockY;
			}
			else {
				commitBlock = true;
			}
			
			m_dropTimer.Start();
		}

		if (commitBlock) {
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

			int numLinesCleared = ClearCompleteLines();
			if (numLinesCleared == 1) {
				m_score += 100;
			}
			else if (numLinesCleared == 2) {
				m_score += 200;
			}
			else if (numLinesCleared == 3) {
				m_score += 400;
			}
			else if (numLinesCleared == 4) {
				m_score += 800;
			}
			m_totalLinesCleared += numLinesCleared;

			m_linesToNextLevel -= numLinesCleared;
			if (m_linesToNextLevel <= 0) {
				++m_level;
				m_linesToNextLevel = m_level * 5;
				m_dropSpeed += 0.5f;
			}

			SpawnTetrimino();

			if (IsPieceColliding(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY)) {
				m_isGameOver = true;
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

		for (size_t y = 0; y < 20; ++y) {
			Line& line = GetLine(y);
			for (size_t x = 0; x < line.contents.size(); ++x) {
				float xPos = m_gridBounds.x + 5.0f + x * m_cellSize;
				float yPos = m_gridBounds.y + 5.0f + y * m_cellSize;

				m_shape.SetPosition(xPos, yPos);
				m_shape.SetSize(m_cellSize, m_cellSize);
				m_shape.SetFillColor(Common::Color::Clear);
				m_shape.SetBorderSizes(0.0f, 1.0f, 1.0f, 0.0f);
				m_renderTarget.Draw(m_shape);
			}
		}

		// Draw the grid
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

		m_shape.SetPosition(m_nextBlockUIBounds.x, m_nextBlockUIBounds.y);
		m_shape.SetSize(m_nextBlockUIBounds.width, m_nextBlockUIBounds.height);
		m_shape.SetBorderColor(Common::Color::White);
		m_shape.SetFillColor(Common::Color::Clear);
		m_shape.SetBorderSizes(5.0f, 5.0f, 5.0f, 5.0f);
		m_renderTarget.Draw(m_shape);

		m_text.SetHorizontalAlignment(Common::Text::HorizontalAlignment::LEFT);
		m_font.SetSize(40);
		m_text.SetString("Next");
		m_text.SetColor(Common::Color::White);

		unsigned int textWidth, textHeight;
		m_text.SetString("Next");
		m_text.ComputeSize(&textWidth, nullptr);
		m_text.SetPosition(
			m_nextBlockUIBounds.x + (m_nextBlockUIBounds.width - textWidth) / 2, 
			m_nextBlockUIBounds.y + m_nextBlockUIBounds.height + 15.0f);
		m_renderTarget.Draw(m_text);

		if (!m_isGameOver) {
			// Draw current tetrimino block we're controlling
			DrawTetrimino(m_tetriminoes[m_tetriminoIndex], m_orientation, m_blockX, m_blockY);	
		}
		
		float nextBlockWidthPixels = m_tetriminoes[m_nextTetriminoIndex].blocks[0].width * m_cellSize;
		float nextBlockHeightPixels = m_tetriminoes[m_nextTetriminoIndex].blocks[0].height * m_cellSize;
		DrawTetrimino2(m_tetriminoes[m_nextTetriminoIndex], 0, 
			m_nextBlockUIBounds.x + (m_nextBlockUIBounds.width - nextBlockWidthPixels) / 2, 
			m_nextBlockUIBounds.y + (m_nextBlockUIBounds.height - nextBlockHeightPixels) / 2);

		int yOffset = 10;
		m_font.SetSize(24);

		m_text.SetString("Score: " + std::to_string(m_score));
		m_text.ComputeSize(&textWidth, &textHeight);
		yOffset += textHeight;
		m_text.SetPosition(m_gridBounds.x + m_gridBounds.width + 30.0f, 
			m_gridBounds.y + m_gridBounds.height - yOffset);
		m_renderTarget.Draw(m_text);

		// Draw lines cleared text
		m_text.SetString("Lines cleared: " + std::to_string(m_totalLinesCleared));
		m_text.ComputeSize(&textWidth, &textHeight);
		yOffset += textHeight + 20.0f;
		m_text.SetPosition(m_gridBounds.x + m_gridBounds.width + 30.0f, 
			m_gridBounds.y + m_gridBounds.height - yOffset);
		m_renderTarget.Draw(m_text);

		m_text.SetString("Level: " + std::to_string(m_level));
		m_text.ComputeSize(&textWidth, &textHeight);
		yOffset += textHeight + 20.0f;
		m_text.SetPosition(m_gridBounds.x + m_gridBounds.width + 30.0f, 
			m_gridBounds.y + m_gridBounds.height - yOffset);
		m_renderTarget.Draw(m_text);

		if (m_isGameOver) {
			m_font.SetSize(40);
			m_text.SetLineSpacing(5.0f);
			m_text.SetString("Game Over\n\nPress space\nto play\nagain");
			m_text.SetHorizontalAlignment(Common::Text::HorizontalAlignment::CENTER);
			m_text.ComputeSize(&textWidth, &textHeight);
			m_text.SetPosition(
				m_gridBounds.x + m_gridBounds.width + 40.0f,
				WINDOW_HEIGHT / 2.0f - 25.0f);
			m_renderTarget.Draw(m_text);
			m_text.SetLineSpacing(0.0f);
		}
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

		//     []
		// [][][]
		lTetrimino.blocks[0].width = 3;
		lTetrimino.blocks[0].height = 2;
		lTetrimino.blocks[0].grid[0][0] = true;
		lTetrimino.blocks[0].grid[1][0] = true;
		lTetrimino.blocks[0].grid[2][0] = true;
		lTetrimino.blocks[0].grid[2][1] = true;

		// []
		// []
		// [][]
		lTetrimino.blocks[1].width = 2;
		lTetrimino.blocks[1].height = 3;
		lTetrimino.blocks[1].grid[0][0] = true;
		lTetrimino.blocks[1].grid[1][0] = true;
		lTetrimino.blocks[1].grid[0][1] = true;
		lTetrimino.blocks[1].grid[0][2] = true;

		// [][][]
		// []
		lTetrimino.blocks[2].width = 3;
		lTetrimino.blocks[2].height = 2;
		lTetrimino.blocks[2].grid[0][0] = true;
		lTetrimino.blocks[2].grid[0][1] = true;
		lTetrimino.blocks[2].grid[1][1] = true;
		lTetrimino.blocks[2].grid[2][1] = true;

		// [][]
		//   []
		//   []
		lTetrimino.blocks[3].width = 2;
		lTetrimino.blocks[3].height = 3;
		lTetrimino.blocks[3].grid[1][0] = true;
		lTetrimino.blocks[3].grid[1][1] = true;
		lTetrimino.blocks[3].grid[1][2] = true;
		lTetrimino.blocks[3].grid[0][2] = true;

		

		return lTetrimino;
	}

	Tetris::Tetrimino TetrisGame::CreateReverseLTetrimino() {
		GameClones::Tetris::Tetrimino rlTetrimino;
		rlTetrimino.color = Common::Color::Magenta;

		// []
		// [][][]
		rlTetrimino.blocks[0].width = 3;
		rlTetrimino.blocks[0].height = 2;
		rlTetrimino.blocks[0].grid[0][0] = true;
		rlTetrimino.blocks[0].grid[0][1] = true;
		rlTetrimino.blocks[0].grid[1][0] = true;
		rlTetrimino.blocks[0].grid[2][0] = true;

		// [][]
		// []
		// []
		rlTetrimino.blocks[1].width = 2;
		rlTetrimino.blocks[1].height = 3;
		rlTetrimino.blocks[1].grid[0][0] = true;
		rlTetrimino.blocks[1].grid[0][1] = true;
		rlTetrimino.blocks[1].grid[0][2] = true;
		rlTetrimino.blocks[1].grid[1][2] = true;

		// [][][]
		//     []
		rlTetrimino.blocks[2].width = 3;
		rlTetrimino.blocks[2].height = 2;
		rlTetrimino.blocks[2].grid[0][1] = true;
		rlTetrimino.blocks[2].grid[1][1] = true;
		rlTetrimino.blocks[2].grid[2][1] = true;
		rlTetrimino.blocks[2].grid[2][0] = true;

		//   []
		//   []
		// [][]
		rlTetrimino.blocks[3].width = 2;
		rlTetrimino.blocks[3].height = 3;
		rlTetrimino.blocks[3].grid[0][0] = true;
		rlTetrimino.blocks[3].grid[1][0] = true;
		rlTetrimino.blocks[3].grid[1][1] = true;
		rlTetrimino.blocks[3].grid[1][2] = true;

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

		// [][][][]
		stickTetrimino.blocks[0].width = 4;
		stickTetrimino.blocks[0].height = 1;
		stickTetrimino.blocks[0].grid[0][0] = true;
		stickTetrimino.blocks[0].grid[1][0] = true;
		stickTetrimino.blocks[0].grid[2][0] = true;
		stickTetrimino.blocks[0].grid[3][0] = true;

		// []
		// []
		// []
		// []
		stickTetrimino.blocks[1].width = 1;
		stickTetrimino.blocks[1].height = 4;
		stickTetrimino.blocks[1].grid[0][0] = true;
		stickTetrimino.blocks[1].grid[0][1] = true;
		stickTetrimino.blocks[1].grid[0][2] = true;
		stickTetrimino.blocks[1].grid[0][3] = true;

		// [][][][]
		stickTetrimino.blocks[2].width = 4;
		stickTetrimino.blocks[2].height = 1;
		stickTetrimino.blocks[2].grid[0][0] = true;
		stickTetrimino.blocks[2].grid[1][0] = true;
		stickTetrimino.blocks[2].grid[2][0] = true;
		stickTetrimino.blocks[2].grid[3][0] = true;

		// []
		// []
		// []
		// []
		stickTetrimino.blocks[3].width = 1;
		stickTetrimino.blocks[3].height = 4;
		stickTetrimino.blocks[3].grid[0][0] = true;
		stickTetrimino.blocks[3].grid[0][1] = true;
		stickTetrimino.blocks[3].grid[0][2] = true;
		stickTetrimino.blocks[3].grid[0][3] = true;

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

	void TetrisGame::DrawTetrimino2(const Tetris::Tetrimino& tetrimino, int orientation, float originX, float originY) {
		const Tetris::Block& block = tetrimino.blocks[orientation];

		float borderSize = 2.0f;

		m_shape.SetBorderColor(Common::Color::White);
		m_shape.SetFillColor(tetrimino.color);

		for (size_t x = 0; x < block.width; ++x) {
			for (size_t y = 0; y < block.height; ++y) {
				float xPos = originX + x * m_cellSize;
				float yPos = originY + y * m_cellSize;
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

	bool TetrisGame::CanPieceMove(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY, int dirX, int dirY) {
		return !IsPieceColliding(tetrimino, orientation, cellX + dirX, cellY + dirY);
	}

	void TetrisGame::Rotate(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		int rotatedOrientation = (orientation + 1) % 4;
		if (cellX + tetrimino.blocks[rotatedOrientation].width >= GRID_NUM_COLUMNS) {
			cellX = GRID_NUM_COLUMNS - tetrimino.blocks[rotatedOrientation].width;
		}

		if (!IsPieceColliding(tetrimino, rotatedOrientation, cellX, cellY)) {
			m_orientation = rotatedOrientation;
			m_blockX = cellX;
		}
	}

	bool TetrisGame::IsPieceColliding(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		const Tetris::Block& block = tetrimino.blocks[orientation];
		if (cellX < 0 || cellX + block.width - 1 >= GRID_NUM_COLUMNS) {
			return true;
		}
		if (cellY < 0) {
			return true;
		}

		for (size_t y = 0; y < block.height; ++y) {
			Line& line = GetLine(cellY + y);
			for (size_t x = 0; x < block.width; ++x) {
				if (block.grid[x][y] && line.contents[cellX + x] != -1) {
					return true;
				}
			}
		}

		return false;
	}

	void TetrisGame::SpawnTetrimino() {
		Common::Random random;
		m_tetriminoIndex = m_nextTetriminoIndex;
		m_nextTetriminoIndex = random(0, m_tetriminoes.size());

		m_orientation = 0;
		m_blockX = (GRID_NUM_COLUMNS - m_tetriminoes[m_tetriminoIndex].blocks[m_orientation].width) / 2;
		m_blockY = 18;
	}

	TetrisGame::Line& TetrisGame::GetLine(int lineIndex) {
		auto it = m_lines.begin();
		while (lineIndex-- > 0) {
			++it;
		}

		return *it;
	}

	int TetrisGame::ClearCompleteLines() {
		int ret = 0;

		for (auto& line : m_lines) {
			if (line.IsFull()) {
				++ret;
			}
		}

		m_lines.remove_if([](TetrisGame::Line& line) { return line.IsFull(); } );
		while (m_lines.size() < GRID_NUM_ROWS) {
			m_lines.emplace_back();
			m_lines.back().contents.resize(GRID_NUM_COLUMNS, -1);
		}

		return ret;
	}

	int TetrisGame::GetYOnDrop(const Tetris::Tetrimino& tetrimino, int orientation, int cellX, int cellY) {
		while (cellY > 0) {
			int nextCellY = cellY - 1;
			if (IsPieceColliding(tetrimino, orientation, cellX, nextCellY)) {
				return cellY;
			}

			--cellY;
		}

		return 0;
	}

	void TetrisGame::Reset() {
		m_lines.clear();

		m_lines.resize(GRID_NUM_ROWS);
		for (auto& line : m_lines) {
			line.contents.resize(GRID_NUM_COLUMNS, -1);
		}

		m_isGameOver = false;

		m_tetriminoIndex = 0;

		m_score = 0;
		m_totalLinesCleared = 0;

		Common::Random random;
		m_nextTetriminoIndex = random(0, m_tetriminoes.size());

		m_blockX = 0;
		m_blockY = 0;
		m_orientation = 0;

		m_level = 1;
		m_linesToNextLevel = m_level * 10;
		m_dropSpeed = 1.0f;
	}
}