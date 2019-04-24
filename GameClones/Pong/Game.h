#pragma once

#ifndef ___GameClones_Pong_Game_h___
#define ___GameClones_Pong_Game_h___

#include "Drawable.h"

#define NUM_PLAYERS 2

#define PADDLE_WIDTH 15.0f
#define PADDLE_HEIGHT 70.0f
#define PADDLE_SPEED 350.0f
#define BALL_RADIUS 5.0f
#define BALL_SPEED 350.0f

#include "Ball.h"
#include "Paddle.h"
#include "MathUtils.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

namespace GameClones {
	namespace Pong {
		class Game : public Drawable {
			Paddle m_paddles[NUM_PLAYERS];
			int m_scores[NUM_PLAYERS];

			std::vector<Ball> m_balls;

			int m_screenWidth;
			int m_screenHeight;

			sf::Font m_font;
			sf::Text m_text;

		public:
			Game(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
			}

			~Game(void) = default;

			void Init() {
				m_paddles[0].SetPosition(0.0f, 0.0f);
				m_paddles[0].SetWidth(PADDLE_WIDTH);
				m_paddles[0].SetHeight(PADDLE_HEIGHT);
				m_paddles[0].SetSpeed(PADDLE_SPEED);

				m_paddles[1].SetPosition(m_screenWidth - PADDLE_WIDTH, 0.0f);
				m_paddles[1].SetWidth(PADDLE_WIDTH);
				m_paddles[1].SetHeight(PADDLE_HEIGHT);
				m_paddles[1].SetSpeed(PADDLE_SPEED);

				m_balls.emplace_back((m_screenWidth - BALL_RADIUS) / 2, (m_screenHeight - BALL_RADIUS) / 2, BALL_RADIUS);
				m_balls[0].SetDirection(-1.0f, -1.0f);
				m_balls[0].SetSpeed(BALL_SPEED);

				m_font.loadFromFile("Resources/Fonts/ArcadeClassic/ARCADECLASSIC.ttf");
				m_text.setFont(m_font);
			}

			void Update(float deltaTime) {
				for (int i = 0; i < NUM_PLAYERS; i++) {
					Paddle& paddle = m_paddles[i];
					paddle.Update(deltaTime);

					Vector2f paddlePos = paddle.GetPosition();
					if (paddlePos.y < 0.0f) {
						paddlePos.y = 0.0f;
					}
					else if (paddlePos.y + paddle.GetHeight() > m_screenHeight) {
						paddlePos.y = m_screenHeight - paddle.GetHeight();
					}

					paddle.SetPosition(paddlePos);
				}

				for (auto& ball : m_balls) {
					ball.Update(deltaTime);

					Vector2f ballPos = ball.GetPosition();
					Vector2f ballDirection = ball.GetDirection();

					if (ballPos.x <= 0.0f) {
						++m_scores[1];

						ballPos.x = (m_screenWidth - BALL_RADIUS) / 2.0f;
						ballPos.y = (m_screenHeight - BALL_RADIUS) / 2.0f;

						ballDirection.x = -ballDirection.x;
					}
					else if (ballPos.x + ball.GetRadius() * 2.0f >= m_screenWidth) {
						++m_scores[0];

						ballPos.x = (m_screenWidth - BALL_RADIUS) / 2.0f;
						ballPos.y = (m_screenHeight - BALL_RADIUS) / 2.0f;

						ballDirection.x = -ballDirection.x;
					}

					if (ballPos.y <= 0.0f) {
						ballPos.y = 0.0f;
						ballDirection.y = -ballDirection.y;
					}
					else if (ballPos.y + ball.GetRadius() * 2.0f >= m_screenHeight) {
						ballPos.y = m_screenHeight - ball.GetRadius() * 2.0f;
						ballDirection.y = -ballDirection.y;
					}

					ball.SetPosition(ballPos);
					ball.SetDirection(ballDirection);
				}

				for (auto& paddle : m_paddles) {
					for (auto& ball : m_balls) {
						// Paddle-ball collision
						if (MathUtils::DoRectanglesIntersect(ball.GetPosition().x, ball.GetPosition().y, ball.GetRadius() * 2, ball.GetRadius() * 2,
							paddle.GetPosition().x, paddle.GetPosition().y, paddle.GetWidth(), paddle.GetHeight())) {
							ball.SetDirection(-ball.GetDirection().x, ball.GetDirection().y);

							if (paddle.GetPosition().x <= ball.GetPosition().x) {
								ball.SetPosition(paddle.GetPosition().x + paddle.GetWidth(), ball.GetPosition().y);
							}
							else if (ball.GetPosition().x <= paddle.GetPosition().x) {
								ball.SetPosition(paddle.GetPosition().x - ball.GetRadius() * 2.0f, ball.GetPosition().y);
							}
						}
					}
				}
			}

			void Draw(sf::RenderTarget& renderTarget) override final {
				for (auto& ball : m_balls) {
					ball.Draw(renderTarget);
				}
				for (auto& paddle : m_paddles) {
					paddle.Draw(renderTarget);
				}

				m_text.setString(std::to_string(m_scores[0]));
				m_text.setCharacterSize(60);
				m_text.setPosition(m_screenWidth / 5 - m_text.getGlobalBounds().width / 2, 40.0f);
				renderTarget.draw(m_text);

				m_text.setString(std::to_string(m_scores[1]));
				m_text.setPosition(m_screenWidth * 4.0f / 5 - m_text.getGlobalBounds().width / 2, 40.0f);
				renderTarget.draw(m_text);
			}

			Ball* GetBall(int index) {
				if (0 <= index && index < m_balls.size()) {
					return &m_balls[index];
				}

				return nullptr;
			}

			int GetNumBalls() const {
				return m_balls.size();
			}

			Paddle* GetPaddle(int index) {
				if (0 <= index && index < NUM_PLAYERS) {
					return &m_paddles[index];
				}

				return nullptr;
			}

			void resetGame() {
				m_paddles[0].SetPosition(0.0f, 0.0f);
				m_paddles[1].SetPosition(m_screenWidth - PADDLE_WIDTH, 0.0f);

				m_balls.clear();
				m_balls.emplace_back((m_screenWidth - BALL_RADIUS) / 2, (m_screenHeight - BALL_RADIUS) / 2, BALL_RADIUS);
				m_balls[0].SetDirection(-1.0f, -1.0f);
				m_balls[0].SetSpeed(BALL_SPEED);

				m_scores[0] = m_scores[1] = 0;
			}
		};
	}
}

#endif // ___GameClones_Pong_Game_h___
