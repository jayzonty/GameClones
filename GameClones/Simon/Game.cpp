#include "Game.h"

#include "MatrixTransform.h"
#include "MathUtils.h"
#include "ListUtils.h"

namespace GameClones {
	namespace Simon {
		Game::Game(unsigned int windowWidth, unsigned int windowHeight, GLFWwindow* window) : 
			m_currentState(State::Start), m_windowWidth(windowWidth), m_windowHeight(windowHeight),
			m_window(window), m_rootScene(nullptr),
			m_memorizeCount(4), m_maxMemorizeCount(9),
			m_memorizeButtonIndex(0), m_level(0) {
		}

		Game::~Game() {
			delete m_rootScene;

			for (size_t i = 0; i < m_memoryButtons.size(); ++i) {
				delete m_memoryButtons[i];
			}
			m_memoryButtons.clear();
		}

		void Game::Init() {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glViewport(0, 0, m_windowWidth, m_windowHeight);

			m_projectionMatrix = Common::MatrixTransform::CreateOrtho(0.0f, m_windowWidth, 0.0f, m_windowHeight);

			m_rootScene = new Common::Scene();

			m_memoryButtons.push_back(new MemoryButton(0, 190.0f, 100.0f, 200.0f, 200.0f, Common::Color(150, 0, 0, 255), Common::Color(255, 0, 0, 255)));
			m_memoryButtons.push_back(new MemoryButton(1, 410.0f, 100.0f, 200.0f, 200.0f, Common::Color(0, 150, 0, 255), Common::Color(0, 255, 0, 255)));
			m_memoryButtons.push_back(new MemoryButton(2, 190.0f, 320.0f, 200.0f, 200.0f, Common::Color(0, 0, 150, 255), Common::Color(0, 0, 255, 255)));
			m_memoryButtons.push_back(new MemoryButton(3, 410.0f, 320.0f, 200.0f, 200.0f, Common::Color(150, 150, 0, 255), Common::Color(255, 255, 0, 255)));

			for (size_t i = 0; i < m_memoryButtons.size(); ++i) {
				m_rootScene->AddDrawable(m_memoryButtons[i]);
			}
			
			m_blinkTimer.SetDuration(1.0f);
			m_cooldownTimer.SetDuration(1.5f);

			m_textShader.Load("Resources/Shaders/Font.vert", "Resources/Shaders/Font.frag");
			m_textRender.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");
			m_textRender.SetFontSize(40);
		}

		void Game::Update(float deltaTime) {
			if (m_currentState == State::Start) {
				
			}
			else if (m_currentState == State::ShowingMemorizeOrder) {
				m_blinkTimer.Update(deltaTime);
				if (!m_blinkTimer.IsRunning() || m_blinkTimer.IsFinished()) {
					if (m_memorizeButtonIndex < m_memorizeCount) {
						int index = m_memorizeOrder[m_memorizeButtonIndex++];
						m_memoryButtons[index]->Blink(3);
						m_blinkTimer.Start();
					}
					else {
						m_memorizeButtonIndex = 0;
						m_currentState = State::WaitingForUserInput;
					}
				}
			}
			else if (m_currentState == State::Cooldown) {
				m_cooldownTimer.Update(deltaTime);
				if (m_cooldownTimer.IsFinished()) {
					m_currentState = State::ShowingMemorizeOrder;
				}
			}

			for (size_t i = 0; i < m_memoryButtons.size(); ++i) {
				m_memoryButtons[i]->Update(deltaTime);
			}
		}

		void Game::Draw() {
			m_rootScene->Draw(m_projectionMatrix);

			m_textShader.Use();
			m_textShader.SetUniformMatrix4f("projection", m_projectionMatrix.mat);

			if (m_currentState != State::Start && m_currentState != State::GameOver) {
				std::string levelStr = "Level " + std::to_string(m_level + 1);
				m_textRender.DrawString(levelStr, 20, 550, Common::Color::Red, m_textShader);
			}
			
			if (m_currentState == State::Start) {
				m_textRender.DrawString("Press any mouse button to play", 20, 20, Common::Color::Red, m_textShader);
			}
			else if (m_currentState == State::ShowingMemorizeOrder) {
				m_textRender.DrawString("Showing Pattern...", 20, 20, Common::Color::Red, m_textShader);
			}
			else if (m_currentState == State::WaitingForUserInput) {
				m_textRender.DrawString("Input the pattern", 20, 20, Common::Color::Red, m_textShader);
			}
			else if (m_currentState == State::GameOver) {
				m_textRender.DrawString("Game Over", 20, 60, Common::Color::Red, m_textShader);
				m_textRender.DrawString("Press any mouse button to play again", 20, 20, Common::Color::Red, m_textShader);
			}
			else if (m_currentState == State::Cooldown) {
				m_textRender.DrawString("Preparing next pattern...", 20, 20, Common::Color::Red, m_textShader);
			}

			m_textShader.Unuse();
		}

		void Game::GenerateMemorizeOrder(size_t memorizeCount) {
			m_memorizeOrder.clear();

			int numCycles = ceil(memorizeCount * 1.0f / m_memoryButtons.size());
			for (int cycle = 0; cycle < numCycles; ++cycle) {
				for (size_t i = 0; i < m_memoryButtons.size(); ++i) {
					m_memorizeOrder.push_back(i);
				}
			}

			ListUtils::Shuffle(m_memorizeOrder);
		}

		void Game::HandleMouseInput(int button, int action, int mods, float mouseX, float mouseY) {
			if (m_currentState == State::Start && action == GLFW_PRESS) {
				GenerateMemorizeOrder(m_memorizeCount);
				m_currentState = State::ShowingMemorizeOrder;
			}
			else if (m_currentState == State::WaitingForUserInput) {
				if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
					for (size_t i = 0; i < m_memoryButtons.size(); ++i) {
						if (m_memoryButtons[i]->GetRect().ContainsPoint(mouseX, 600 - mouseY)) {
							m_memoryButtons[i]->Blink();

							if (m_memoryButtons[i]->GetId() != m_memorizeOrder[m_memorizeButtonIndex]) {
								m_currentState = State::GameOver;
							}
							else if (++m_memorizeButtonIndex == m_memorizeCount) {
								++m_level;
								if ((m_level % 5 == 0) && (m_memorizeCount < m_maxMemorizeCount)) {
									m_memorizeCount++;
								}

								m_memorizeButtonIndex = 0;
								GenerateMemorizeOrder(m_memorizeCount);

								m_cooldownTimer.Start();
								m_currentState = State::Cooldown;
							}
						}
					}
				}
			}
			else if (m_currentState == State::GameOver && action == GLFW_PRESS) {
				Reset();
				m_currentState = State::ShowingMemorizeOrder;
			}
		}

		void Game::Reset() {
			m_level = 0;
			m_memorizeButtonIndex = 0;
			m_memorizeCount = 4;
		}
	}
}