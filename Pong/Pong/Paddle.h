#pragma once

#ifndef ___GameClones_Pong_Paddle_h___
#define ___GameClones_Pong_Paddle_h___

#include "Drawable.h"
#include "Vector2.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace GameClones {
	namespace Pong {
		class Paddle : public Drawable {
			sf::RectangleShape m_shape;

			Vector2f m_position;
			Vector2f m_direction;
			float m_speed;
		public:
			Paddle() : m_shape(), m_position(0.0f, 0.0f), m_direction(0.0f, 0.0f), m_speed(0.0f) {
				m_shape.setPosition(m_position.x, m_position.y);
				m_shape.setFillColor(sf::Color::White);
			}

			void Update(float deltaTime) {
				m_position += m_direction * m_speed * deltaTime;
				m_shape.setPosition(m_position.x, m_position.y);
			}

			void Draw(sf::RenderTarget& renderTarget) override final {
				renderTarget.draw(m_shape);
			}

			/*----------
			* Getters and setters
			*/
			Vector2f GetPosition() const {
				return m_position;
			}

			void SetPosition(float x, float y) {
				m_position.x = x;
				m_position.y = y;

				m_shape.setPosition(x, y);
			}

			void SetPosition(Vector2f& position) {
				SetPosition(position.x, position.y);
			}

			Vector2f GetDirection() const {
				return m_direction;
			}

			void SetDirection(float dx, float dy) {
				m_direction.x = dx;
				m_direction.y = dy;
			}

			void SetDirection(Vector2f& direction) {
				m_direction = direction;
			}

			float GetWidth() const {
				return m_shape.getSize().x;
			}

			void SetWidth(float width) {
				m_shape.setSize(sf::Vector2f(width, m_shape.getSize().y));
			}

			float GetHeight() const {
				return m_shape.getSize().y;
			}

			void SetHeight(float height) {
				m_shape.setSize(sf::Vector2f(m_shape.getSize().x, height));
			}

			float GetSpeed() const {
				return m_speed;
			}

			void SetSpeed(float speed) {
				m_speed = speed;
			}
		};
	}
}

#endif // ___GameClones_Pong_Paddle_h___
