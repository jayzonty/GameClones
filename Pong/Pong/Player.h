#pragma once

#ifndef ___GameClones_Pong_Player_h___
#define ___GameClones_Pong_Player_h___

#include "Vector2.h"
#include "Game.h"

#include <SFML/Window.hpp>

namespace GameClones {
	namespace Pong {
		class Player {
			int m_index;

		public:
			Player(int index) : m_index(index) {}

			virtual void Update(float deltaTime, Game& game) = 0;

			void setIndex(int index) {
				m_index = index;
			}

			int getIndex() const {
				return m_index;
			}
		};

		class HumanPlayer : public Player {
		public:
			HumanPlayer(int index) : Player(index) {}

			void Update(float deltaTime, Game& game) override final {
				Vector2f paddleDirection = game.GetPaddle(getIndex())->GetDirection();
				
				if (((getIndex() == 0) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || ((getIndex() == 1) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {
					paddleDirection.y = -1.0f;
				}
				else if (((getIndex() == 0) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) || ((getIndex() == 1) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
					paddleDirection.y = 1.0f;
				}
				else {
					paddleDirection.y = 0.0f;
				}

				game.GetPaddle(getIndex())->SetDirection(paddleDirection);
			}
		};

		// Technically, this is a dumb AI since the AI should consider the position
		// of the ball relative to the paddle, but whatever.
		class AIPlayer : public Player {
		public:
			AIPlayer(int index) : Player(index) {}

			void Update(float deltaTime, Game& game) override final {
				Paddle* paddle = game.GetPaddle(getIndex());

				Vector2f paddleDirection = paddle->GetDirection();
				Vector2f ballDirection = game.GetBall(0)->GetDirection();

				Vector2f paddlePos = paddle->GetPosition();
				Vector2f ballPos = game.GetBall(0)->GetPosition();

				// If the ball is heading towards our paddle, move the paddle
				if ((paddlePos.x - ballPos.x) * ballDirection.x > 0.0f) {
					paddleDirection.y = ballDirection.y;
				}
				else {
					paddleDirection.y = 0.0f;
				}

				game.GetPaddle(getIndex())->SetDirection(paddleDirection);
			}
		};
	}
}

#endif // ___GameClones_Pong_Player_h___
