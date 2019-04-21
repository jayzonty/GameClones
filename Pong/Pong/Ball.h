#pragma once

#ifndef ___GameClones_Pong_Ball_h___
#define ___GameClones_Pong_Ball_h___

#include "Drawable.h"
#include "Vector2.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace GameClones {
	namespace Pong {
		class Ball : public Drawable {
			sf::CircleShape m_shape;

			Vector2f m_position;
			Vector2f m_direction;
			float m_speed;
		public:
			Ball(float x, float y, float radius) : m_shape(radius), m_position(x, y), m_direction(0.0f, 0.0f), m_speed(0.0f) {
				m_shape.setPosition(x, y);
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
			float GetRadius() {
				return m_shape.getRadius();
			}

			void SetRadius(float radius) {
				m_shape.setRadius(radius);
			}

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

			void SetDirection(Vector2f& velocity) {
				m_direction = velocity;
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

#endif // ___GameClones_Pong_Ball_h___
