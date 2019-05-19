#include "Nibbles.h"

#include "Common/WindowManager.h"
#include "Common/Window.h"

#include "Common/Input.h"
#include "Common/Random.h"

#include "Common/Math.h"

namespace GameClones {
	namespace Nibbles {
		Nibbles::Nibbles() : Common::BaseApplication("Nibbles"),
			m_currentState(State::Start),
			m_numRows(0), m_numColumns(0),
			m_inputBufferLimit(2),
			m_foodX(0), m_foodY(0),
			m_snakeBodyColor(Common::Color::White),
			m_snakeHeadColor(Common::Color::Blue),
			m_foodColor(Common::Color::Red),
			m_snakeDirX(0), m_snakeDirY(0),
			m_snakeSpeed(0),
			m_score(0) {
		}

		Nibbles::~Nibbles() {
		}

		void Nibbles::Init() {
			// Setup window
			Common::Window* window = Common::WindowManager::GetMainWindow();
			window->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
			window->SetTitle("Nibbles");

			// Setup render target
			m_renderTarget.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

			// Setup game bounds
			m_gameBounds = Common::Rect<float>(5.0f, 5.0f, WINDOW_WIDTH - 10.0f, WINDOW_HEIGHT - 100.0f);

			// Setup cell sizes
			m_cellSize = 10.0f;

			m_numColumns = static_cast<int>(m_gameBounds.width / m_cellSize);
			m_numRows = static_cast<int>(m_gameBounds.height / m_cellSize);

			m_rectangleShape.SetSize(m_cellSize, m_cellSize);

			m_gameBoundsBorderShape.SetPosition(0.0f, 0.0f);
			m_gameBoundsBorderShape.SetSize(WINDOW_WIDTH * 1.0f, WINDOW_HEIGHT - 90.0f);
			m_gameBoundsBorderShape.SetFillColor(Common::Color::White);

			m_gameBoundsShape.SetPosition(m_gameBounds.x, m_gameBounds.y);
			m_gameBoundsShape.SetSize(m_gameBounds.width, m_gameBounds.height);
			m_gameBoundsShape.SetFillColor(Common::Color::Black);

			InitializeGrid();

			m_movementTickTimer.SetDuration(0.05f);
			m_movementTickTimer.Start();

			ResetSnake();

			m_textFont.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");
			m_textFont.SetSize(30);

			m_statusText.SetFont(m_textFont);
			m_statusText.SetColor(Common::Color::White);
			
			m_snakeSpeed = 50; // cells per second
			m_movementTickTimer.SetDuration(1.0f / m_snakeSpeed);
			m_movementTickTimer.Start();
		}

		void Nibbles::Update(float deltaTime) {
			if (m_currentState == State::Start) {
				int gameResult = UpdateGame(deltaTime);

				int minSpeed = 30;
				int maxSpeed = 80;
				int speedIncrement = 5;
				if (Common::Input::IsPressed(Common::Input::SPACE)) {
					Reset();
					m_currentState = State::Play;
					
					m_movementTickTimer.Stop();
					m_movementTickTimer.Start();
				}
				else if (Common::Input::IsPressed(Common::Input::UP)) {
					m_snakeSpeed = Common::Math::Min(m_snakeSpeed + speedIncrement, maxSpeed); 
					m_movementTickTimer.SetDuration(1.0f / m_snakeSpeed);

					m_movementTickTimer.Stop();
					m_movementTickTimer.Start();
				}
				else if (Common::Input::IsPressed(Common::Input::DOWN)) {
					m_snakeSpeed = Common::Math::Max(m_snakeSpeed - speedIncrement, minSpeed);
					m_movementTickTimer.SetDuration(1.0f / m_snakeSpeed);

					m_movementTickTimer.Stop();
					m_movementTickTimer.Start();
				}
				
				if (gameResult == -1 || gameResult == 1) {
					Reset();
				}
			}
			else if (m_currentState == State::Play) {
				if (Common::Input::IsPressed(Common::Input::ESC)) {
					m_currentState = State::Start;
					Reset();
				}
				else {
					// Handle input
					// Prevent player from moving to itself, thus
					// the additional direction checks
					if (m_inputBuffer.size() < m_inputBufferLimit) {
						const IntVec2& head = m_snakeBody.front();
						int dirX = m_snakeDirX;
						int dirY = m_snakeDirY;

						if (!m_inputBuffer.empty()) {
							dirX = m_inputBuffer.back().x;
							dirY = m_inputBuffer.back().y;
						}

						if (Common::Input::IsPressed(Common::Input::UP) && (dirY != -1)) {
							m_inputBuffer.push_back({ 0, 1 });
						}
						else if (Common::Input::IsPressed(Common::Input::DOWN) && (dirY != 1)) {
							m_inputBuffer.push_back({ 0, -1 });
						}
						else if (Common::Input::IsPressed(Common::Input::LEFT) && (dirX != 1)) {
							m_inputBuffer.push_back({ -1, 0 });
						}
						else if (Common::Input::IsPressed(Common::Input::RIGHT) && (dirX != -1)) {
							m_inputBuffer.push_back({ 1, 0 });
						}
					}

					int result = UpdateGame(deltaTime);
					if (result == -1) {
						m_currentState = State::Lose;
					}
					else if (result == 1) {
						m_currentState = State::Win;
					}
				}
			}
			else if (m_currentState == State::Lose || m_currentState == State::Win) {
				if (Common::Input::IsPressed(Common::Input::SPACE)) {
					m_currentState = State::Play;
					Reset();
				}
				else if (Common::Input::IsPressed(Common::Input::ESC)) {
					m_currentState = State::Start;
					Reset();
				}
			}
		}

		void Nibbles::Draw() {
			m_renderTarget.Draw(m_gameBoundsBorderShape);
			m_renderTarget.Draw(m_gameBoundsShape);

			// Draw the snake
			bool isHead = true;
			for (const IntVec2& body : m_snakeBody) {
				if (isHead) {
					m_rectangleShape.SetFillColor(m_snakeHeadColor);
					isHead = false;
				}
				else {
					m_rectangleShape.SetFillColor(m_snakeBodyColor);
				}

				m_rectangleShape.SetPosition(
					m_gameBounds.x + body.x * m_cellSize, 
					m_gameBounds.y + body.y * m_cellSize);
				m_renderTarget.Draw(m_rectangleShape);
			}

			// Draw the food
			if (m_foodX != -1 && m_foodY != -1) {
				m_rectangleShape.SetFillColor(m_foodColor);
				m_rectangleShape.SetPosition(
					m_gameBounds.x + m_foodX * m_cellSize, 
					m_gameBounds.y + m_foodY * m_cellSize);
				m_renderTarget.Draw(m_rectangleShape);
			}

			unsigned int textWidth, textHeight;
			float textPositionY = m_gameBounds.y + m_gameBounds.height + 20.0f;
			if (m_currentState == State::Start) {
				m_statusText.SetString("Press up/down arrow to increase/decrease speed: " + std::to_string(m_snakeSpeed));
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					(WINDOW_WIDTH - textWidth) / 2.0f, 
					textPositionY);
				m_renderTarget.Draw(m_statusText);

				m_statusText.SetString("Press space to play");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					(WINDOW_WIDTH - textWidth) / 2.0f,
					textPositionY + textHeight + 15.0f);
				m_renderTarget.Draw(m_statusText);
			}
			else if (m_currentState == State::Play) {
				m_statusText.SetString("Press ESC to go back to main menu");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					(WINDOW_WIDTH - textWidth) / 2.0f, 
					textPositionY);
				m_renderTarget.Draw(m_statusText);

				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(m_gameBounds.x, WINDOW_HEIGHT - textHeight - 25.0f);
				m_statusText.SetString("Score: " + std::to_string(m_score));
				m_renderTarget.Draw(m_statusText);
			}
			else if (m_currentState == State::Lose) {
				m_statusText.SetString("Press space to play again. Press ESC to go back to main menu");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					m_gameBounds.x + (m_gameBounds.width - textWidth) / 2,
					textPositionY);
				m_renderTarget.Draw(m_statusText);

				m_statusText.SetString("You lost (Score: " + std::to_string(m_score) + ")");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					(WINDOW_WIDTH - textWidth) / 2, 
					textPositionY + textHeight + 15.0f);
				m_renderTarget.Draw(m_statusText);
			}
			else if (m_currentState == State::Win) {
				m_statusText.SetString("Press space to play again. Press ESC to go back to main menu");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					m_gameBounds.x + (m_gameBounds.width - textWidth) / 2,
					textPositionY);
				m_renderTarget.Draw(m_statusText);

				m_statusText.SetString("You won (Score: " + std::to_string(m_score) + ")");
				m_statusText.ComputeSize(&textWidth, &textHeight);
				m_statusText.SetPosition(
					(WINDOW_WIDTH - textWidth) / 2, 
					textPositionY + textHeight + 10.0f);
				m_renderTarget.Draw(m_statusText);
			}
		}

		void Nibbles::Cleanup() {
		}

		void Nibbles::InitializeGrid() {
			unsigned int numCells = m_numRows * m_numColumns;
			m_grid.resize(numCells, 0);
		}

		void Nibbles::ClearGrid() {
			for (size_t i = 0; i < m_grid.size(); ++i) {
				m_grid[i] = 0;
			}
		}

		void Nibbles::ProcessInputBuffer() {
			if (!m_inputBuffer.empty()) {
				IntVec2 input = m_inputBuffer.front();
				m_snakeDirX = input.x;
				m_snakeDirY = input.y;
				m_inputBuffer.pop_front();
			}
		}

		// Returns -1 if player lost
		// Returns 1 if player won
		// Returns 0 otherwise
		int Nibbles::UpdateGame(float deltaTime) {
			m_movementTickTimer.Update(deltaTime);
			if (m_movementTickTimer.IsFinished()) {
				ProcessInputBuffer();
				int collidedCellType = UpdateSnakePosition(deltaTime);
				if (collidedCellType == 1) { // Collided with self
					return -1;
				}
				else if (collidedCellType == 2) { // Collided with food
					ExtendSnake(1);

					unsigned int totalCells = m_numRows * m_numColumns;
					if (m_snakeBody.size() == totalCells) {
						return 1;
					}

					SpawnFood();

					++m_score;
				}

				m_movementTickTimer.Start();
			}

			return 0;
		}

		void Nibbles::Reset() {
			m_score = 0;

			ClearGrid();
			ResetSnake();
			SpawnFood();
		}

		void Nibbles::ResetSnake() {
			m_snakeBody.clear();

			Common::Random random;
			IntVec2 snakeHead;
			snakeHead.x = random(0, m_numColumns);
			snakeHead.y = random(0, m_numRows);
			GetRandomDirection(&m_snakeDirX, &m_snakeDirY);
			m_snakeBody.push_back(snakeHead);
			SetCellType(snakeHead.x, snakeHead.y, 1);

			ExtendSnake(2);
		}

		// Returns the type of cell that the head of the snake falls onto
		// (i.e., the type of the cell prior to being overwritten by the snake head)
		// 0 = empty, 1 = snake body part, 2 = food
		int Nibbles::UpdateSnakePosition(float deltaTime) {
			int newX = m_snakeBody.front().x + m_snakeDirX;
			int newY = m_snakeBody.front().y + m_snakeDirY;
			WrapCell(&newX, &newY);

			int ret = GetCellType(newX, newY);

			for (IntVec2& bodyPart : m_snakeBody) {
				int prevX = bodyPart.x;
				int prevY = bodyPart.y;

				bodyPart.x = newX;
				bodyPart.y = newY;

				SetCellType(prevX, prevY, 0);
				SetCellType(newX, newY, 1);

				newX = prevX;
				newY = prevY;
			}

			return ret;
		}

		void Nibbles::ExtendSnake(unsigned int amount) {
			IntVec2 newPart = m_snakeBody.back();
			for (unsigned int i = 0; i < amount; ++i) {
				m_snakeBody.push_back(newPart);
				SetCellType(newPart.x, newPart.y, 1);
			}
		}

		// Returns true if successful in determining a position for the food
		// (i.e., still has empty cells)
		bool Nibbles::SpawnFood() {
			Common::Random random;
			int foodX = random(0, m_numColumns);
			int foodY = random(0, m_numRows);

			for (unsigned int x = 0; x < m_numColumns; ++x) {
				for (unsigned int y = 0; y < m_numRows; ++y) {
					unsigned int tempX = (foodX + x) % m_numColumns;
					unsigned int tempY = (foodY + y) % m_numRows;
					
					if (GetCellType(tempX, tempY) == 0) {
						m_foodX = tempX;
						m_foodY = tempY;
						SetCellType(tempX, tempY, 2);

						return true;
					}
				}
			}

			m_foodX = -1;
			m_foodY = -1;

			return false;
		}

		void Nibbles::SetCellType(int x, int y, unsigned char type) {
			unsigned int index = y * m_numColumns + x;
			m_grid[index] = type;
		}

		unsigned char Nibbles::GetCellType(int x, int y) {
			unsigned int index = y * m_numColumns + x;
			return m_grid[index];
		}

		void Nibbles::WrapCell(int* cellX, int* cellY) {
			*cellX = (*cellX + m_numColumns) % m_numColumns;
			*cellY = (*cellY + m_numRows) % m_numRows;
		}

		void Nibbles::GetDirectionFromIndex(int index, int* directionX, int* directionY) {
			switch (index) {
			case 0: // Up
				*directionX = 0;
				*directionY = 1;
				break;
			case 1: // Right
				*directionX = 1;
				*directionY = 0;
				break;
			case 2: // Down
				*directionX = 0;
				*directionY = -1;
				break;
			case 3: // Left
				*directionX = -1;
				*directionY = 0;
				break;
			default:
				break;
			}
		}

		void Nibbles::GetRandomDirection(int* directionX, int* directionY) {
			Common::Random random;
			int index = random(0, 4);
			GetDirectionFromIndex(index, directionX, directionY);
		}
	}
}