#pragma once

#ifndef ___GameClones_MemoryPuzzle_MemoryPanel_h___
#define ___GameClones_MemoryPuzzle_MemoryPanel_h___

#include <SFML/Graphics.hpp>

#include "MathUtils.h"

namespace GameClones {
	namespace MemoryPuzzle {
		class MemoryPanel {
			int m_id;

			sf::Sprite m_sprite;
			sf::RectangleShape m_back;

			sf::FloatRect m_rect;
			bool m_isFlipping;

			float m_scaleX;
			float m_scaleY;

			float m_flipDuration;
			float m_flipTime;

			bool m_isFacingUp;

		public:
			MemoryPanel(int id, sf::Sprite sprite) : m_id(id), m_sprite(sprite), m_rect(), m_isFlipping(false), m_scaleX(1.0f), m_scaleY(1.0f), m_flipDuration(0.5f), m_flipTime(0.0f), m_isFacingUp(false) {
				m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
				
				m_back.setOutlineColor(sf::Color::Black);
				m_back.setOutlineThickness(5.0f);
				m_back.setFillColor(sf::Color::Blue);
			}

			void SetSize(float width, float height) {
				m_rect.width = width;
				m_rect.height = height;

				m_scaleX = width / m_sprite.getTextureRect().width;
				m_scaleY = height / m_sprite.getTextureRect().height;

				m_sprite.setScale(m_scaleX, m_scaleY);

				m_back.setSize(sf::Vector2f(width, height));
				m_back.setOrigin(width / 2, height / 2);
			}

			void SetPosition(float x, float y) {
				m_rect.left = x;
				m_rect.top = y;
			}

			void Draw(sf::RenderTarget& renderTarget) {
				m_sprite.setPosition(m_rect.left + m_sprite.getOrigin().x * m_scaleX, m_rect.top + m_sprite.getOrigin().y * m_scaleY);
				m_back.setPosition(m_rect.left + m_back.getSize().x / 2, m_rect.top + m_back.getSize().y / 2);

				if (m_isFacingUp) {
					renderTarget.draw(m_sprite);
				}
				else {
					renderTarget.draw(m_back);
				}
			}

			void Highlight() {
				if (!m_isFacingUp) {
					m_back.setOutlineColor(sf::Color::Yellow);
				}
			}

			void Unhighlight() {
				if (!m_isFacingUp) {
					m_back.setOutlineColor(sf::Color::Black);
				}
			}

			void Update(float deltaTime) {
				if (m_isFlipping) {
					float prevFlipTime = m_flipTime;
					m_flipTime = MathUtils::Clamp(m_flipTime - deltaTime, 0.0f, m_flipDuration);
					float scaleModifier = abs(-1.0f + m_flipTime / m_flipDuration * 2.0f);
					if (m_flipTime == 0.0f) {
						m_isFlipping = false;
					}
					else if ((m_flipTime <= m_flipDuration / 2.0f) && (prevFlipTime > m_flipDuration / 2.0f)) {
						m_isFacingUp = !m_isFacingUp;
					}
					
					m_sprite.setScale(m_scaleX * scaleModifier, m_scaleY);
					m_back.setScale(scaleModifier, 1.0f);
				}
			}

			void Flip() {
				if (!m_isFlipping) {
					m_isFlipping = true;
					m_flipTime = m_flipDuration;
				}

				Unhighlight();
			}

			void SetIsFacingUp(bool isFacingUp) {
				m_isFacingUp = isFacingUp;
			}

			bool IsFacingUp() const {
				return m_isFacingUp;
			}

			sf::FloatRect GetRect() {
				return m_rect;
			}

			bool IsFlipping() const {
				return m_isFlipping;
			}

			int GetId() const {
				return m_id;
			}
		};
	}
}

#endif // ___GameClones_MemoryPuzzle_MemoryPanel_h___
