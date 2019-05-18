#include "SlidingPuzzle.h"

#include "Common/Input.h"
#include "Common/Random.h"

#include "Common/WindowManager.h"
#include "Common/Window.h"

namespace GameClones {
	namespace SlidingPuzzle {
		SlidingPuzzle::SlidingPuzzle() : 
			Common::BaseApplication("SlidingPuzzle"), 
			m_currentState(State::Start),
			m_rows(4), m_columns(4) {
		}

		SlidingPuzzle::~SlidingPuzzle() {
			for (size_t i = 0; i < m_panelGrid.size(); ++i) {
				delete m_panelGrid[i];
			}
		}

		void SlidingPuzzle::Init() {
			Common::Window* mainWindow = Common::WindowManager::GetMainWindow();
			mainWindow->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
			mainWindow->SetTitle("Sliding Puzzle");

			m_renderTarget.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

			m_gridBounds.x = 100;
			m_gridBounds.y = 100;
			m_gridBounds.width = WINDOW_WIDTH - 200;
			m_gridBounds.height = WINDOW_HEIGHT - 200;
			
			for (unsigned int i = 0; i < m_rows * m_columns; ++i) {
				m_panelGrid.push_back(new Panel((i + 1) % (m_rows * m_columns)));
				m_panelGrid.back()->SetFont(m_textFont);

				m_panelGridCopy.push_back(m_panelGrid.back());
			}

			m_blankRow = m_blankRowCopy = m_rows - 1;
			m_blankColumn = m_blankColumnCopy = m_columns - 1;

			m_backgroundShape.SetPosition(m_gridBounds.x, m_gridBounds.y);
			m_backgroundShape.SetSize(m_gridBounds.width, m_gridBounds.height);
			m_backgroundShape.SetFillColor(Common::Color::Blue);

			m_textFont.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");
			m_textFont.SetSize(40);

			m_statusText.SetPosition(100, 725);
			m_statusText.SetFont(m_textFont);
			m_statusText.SetString("Press New Game to play");
			m_statusText.SetColor(Common::Color::White);

			m_newGameButton.SetFont(m_textFont);
			m_newGameButton.SetPosition(100, 30);
			m_newGameButton.SetSize(200, 50);
			m_newGameButton.SetText("New Game");

			m_resetButton.SetFont(m_textFont);
			m_resetButton.SetPosition(320, 30);
			m_resetButton.SetSize(200, 50);
			m_resetButton.SetText("Reset");
		}

		void SlidingPuzzle::Update(float deltaTime) {
			if (m_currentState == State::Play) {
				if (Common::Input::IsPressed(Common::Input::UP)) {
					PressUp();
					CheckWin();
				}
				else if (Common::Input::IsPressed(Common::Input::DOWN)) {
					PressDown();
					CheckWin();
				}
				else if (Common::Input::IsPressed(Common::Input::LEFT)) {
					PressLeft();
					CheckWin();
				}
				else if (Common::Input::IsPressed(Common::Input::RIGHT)) {
					PressRight();
					CheckWin();
				}
			}

			int mouseX, mouseY;
			Common::Input::GetMousePosition(&mouseX, &mouseY);
			mouseY = WINDOW_HEIGHT - mouseY;

			if (Common::Input::IsPressed(Common::Input::LEFT_MOUSE)) {	
				if (m_newGameButton.GetBounds().ContainsPoint(mouseX, mouseY)) {
					m_currentState = State::Play;
					Shuffle();
				}
				else if (m_resetButton.GetBounds().ContainsPoint(mouseX, mouseY)) {
					ResetGrid();
				}
			}

			if (m_newGameButton.GetBounds().ContainsPoint(mouseX, mouseY)) {
				m_newGameButton.Hover();
			}
			else {
				m_newGameButton.Unhover();
			}

			if (m_resetButton.GetBounds().ContainsPoint(mouseX, mouseY)) {
				m_resetButton.Hover();
			}
			else {
				m_resetButton.Unhover();
			}
		}

		void SlidingPuzzle::Draw() {
			m_renderTarget.Draw(m_backgroundShape);

			unsigned int borderSize = 10;
			unsigned int panelWidth = (m_gridBounds.width - borderSize * (m_columns + 1)) / m_columns;
			unsigned int panelHeight = (m_gridBounds.height - borderSize * (m_rows + 1)) / m_rows;

			int x = m_gridBounds.x + borderSize;
			int y = m_gridBounds.y + m_gridBounds.height - panelHeight - borderSize;
			for (int row = 0; row < 4; ++row) {
				x = m_gridBounds.x + borderSize;
				for (int column = 0; column < 4; ++column) {
					int index = row * 4 + column;

					Panel* panel = m_panelGrid[index];
					panel->SetPosition(x, y);
					panel->SetSize(panelWidth, panelHeight);

					x += panelWidth + borderSize;
				}
				y -= panelHeight + borderSize;
			}

			for (size_t i = 0; i < m_panelGrid.size(); ++i) {
				m_panelGrid[i]->Draw(m_renderTarget);
			}

			if (m_currentState != State::Play) {
				unsigned int textWidth, textHeight;
				m_statusText.ComputeSize(&textWidth, &textHeight);

				m_statusText.SetPosition(
					m_gridBounds.x + (m_gridBounds.width - textWidth) / 2,
					725 + textHeight / 2);

				m_renderTarget.Draw(m_statusText);
			}

			m_renderTarget.Draw(m_newGameButton);

			if (m_currentState == State::Play) {
				m_renderTarget.Draw(m_resetButton);
			}
		}

		void SlidingPuzzle::Cleanup() {
		}

		void SlidingPuzzle::Shuffle() {
			Common::Random random;
			// Not sure if simply shuffling the m_panelGrid
			// list will always yield a solvable grid, so we just
			// simulate randomly pressing buttons to shuffle the list
			int iterations = 100;
			while (iterations > 0) {
				int action = random(0, 4);
				if ((action == 0 && PressUp()) ||
					(action == 1 && PressDown()) || 
					(action == 2 && PressLeft()) ||
					(action == 3 && PressRight())) {
					--iterations;
				}
			}

			m_panelGridCopy = m_panelGrid;
			GetBlankCellPosition(m_panelGrid, m_blankRowCopy, m_blankColumnCopy);
		}

		void SlidingPuzzle::ResetGrid() {
			m_panelGrid = m_panelGridCopy;

			m_blankRow = m_blankRowCopy;
			m_blankColumn = m_blankColumnCopy;
		}

		void SlidingPuzzle::CheckWin() {
			size_t length = m_rows * m_columns;
			for (size_t i = 0; i < length; ++i) {
				int id = m_panelGrid[i]->GetId();
				if (id != ((i + 1) % length)) {
					return;
				}
			}

			m_currentState = State::Win;
			m_statusText.SetString("You win");
		}

		int SlidingPuzzle::GetIndex(int row, int column) {
			return row * m_columns + column;
		}

		void SlidingPuzzle::SwapPanels(
			int panel1Row, int panel1Column, 
			int panel2Row, int panel2Column) {
			int panel1Index = GetIndex(panel1Row, panel1Column);
			int panel2Index = GetIndex(panel2Row, panel2Column);

			Panel* temp = m_panelGrid[panel1Index];
			m_panelGrid[panel1Index] = m_panelGrid[panel2Index];
			m_panelGrid[panel2Index] = temp;
		}

		bool SlidingPuzzle::CanPressUp() {
			return (m_blankRow < m_rows - 1);
		}

		bool SlidingPuzzle::CanPressDown() {
			return (m_blankRow > 0);
		}

		bool SlidingPuzzle::CanPressLeft() {
			return (m_blankColumn < m_columns - 1);
		}

		bool SlidingPuzzle::CanPressRight() {
			return (m_blankColumn > 0);
		}

		bool SlidingPuzzle::PressUp() {
			// Move the cell below the blank cell upwards
			// Note: column 0 is rendered at the top
			if (CanPressUp()) {
				SwapPanels(m_blankRow, m_blankColumn, m_blankRow + 1, m_blankColumn);
				++m_blankRow;
				return true;
			}
			return false;
		}

		bool SlidingPuzzle::PressDown() {
			// Move the cell above the blank cell downwards
			// Note: column 0 is rendered at the top
			if (CanPressDown()) {
				SwapPanels(m_blankRow, m_blankColumn, m_blankRow - 1, m_blankColumn);
				--m_blankRow;
				return true;
			}
			return false;
		}

		bool SlidingPuzzle::PressLeft() {
			// Move the cell that is to the right of the blank cell to the left
			if (CanPressLeft()) {
				SwapPanels(m_blankRow, m_blankColumn, m_blankRow, m_blankColumn + 1);
				++m_blankColumn;
				return true;
			}
			return false;
		}

		bool SlidingPuzzle::PressRight() {
			// Move the cell that is to the left of the blank cell to the right
			if (CanPressRight()) {
				SwapPanels(m_blankRow, m_blankColumn, m_blankRow, m_blankColumn - 1);
				--m_blankColumn;
				return true;
			}
			return false;
		}

		void SlidingPuzzle::GetBlankCellPosition(const std::vector<Panel*>& grid, unsigned int& row, unsigned int& column) {
			for (size_t r = 0; r < m_rows; ++r) {
				for (size_t c = 0; c < m_columns; ++c) {
					int index = GetIndex(r, c);
					if (grid[index]->GetId() == 0) {
						row = r;
						column = c;
						return;
					}
				}
			}
		}
	}
}