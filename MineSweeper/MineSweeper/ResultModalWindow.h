#pragma once

#ifndef ___GameClones_ModalWindow_h___
#define ___GameClones_ModalWindow_h___

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>
#include <functional>

#include "MathUtils.h"

namespace GameClones {
	class ResultModalWindow {
		sf::Vector2f m_position;
		sf::Vector2f m_size;

		sf::Vector2f m_buttonPosition;
		sf::Vector2f m_buttonSize;

		sf::RectangleShape m_shape;

		bool m_isVisible;
		bool m_isButtonHighlighted;

		sf::Text m_text;
		std::string m_textString;

		std::function<void()> m_confirmClickedCallback;

	public:
		ResultModalWindow() : m_position(0.0f, 0.0f), m_size(0.0f, 0.0f), m_isVisible(false), m_isButtonHighlighted(false) {}

		~ResultModalWindow() {}

		void HandleEvent(sf::Event& event) {
			if (m_isVisible) {
				if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
					if (MathUtils::IsPointInsideAABBRectangle(event.mouseButton.x, event.mouseButton.y, m_buttonPosition.x, m_buttonPosition.y, m_buttonSize.x, m_buttonSize.y)) {
						m_isVisible = false;
						m_isButtonHighlighted = false;
						m_confirmClickedCallback();
					}
				}
				else if (event.type == sf::Event::MouseMoved) {
					m_isButtonHighlighted = MathUtils::IsPointInsideAABBRectangle(event.mouseMove.x, event.mouseMove.y, m_buttonPosition.x, m_buttonPosition.y, m_buttonSize.x, m_buttonSize.y);
				}
			}
		}

		void Draw(sf::RenderTarget& renderTarget) {
			if (m_isVisible) {
				// Draw outer window
				m_shape.setFillColor(sf::Color::Black);
				m_shape.setPosition(m_position);
				m_shape.setSize(m_size);
				m_shape.setOutlineColor(sf::Color::White);
				m_shape.setOutlineThickness(10);
				renderTarget.draw(m_shape);

				// Draw text
				m_text.setFillColor(sf::Color::White);
				m_text.setCharacterSize(50);
				m_text.setString(m_textString);
				sf::FloatRect textBounds = m_text.getLocalBounds();
				m_text.setPosition(m_position.x + (m_size.x - textBounds.width) / 2, m_position.y + m_size.y / 5 - textBounds.top);
				renderTarget.draw(m_text);

				// Draw play again button
				m_text.setCharacterSize(30);
				m_text.setString("Play again");
				m_text.setFillColor(m_isButtonHighlighted ? sf::Color::Black : sf::Color::White);

				m_shape.setOutlineThickness(6);
				m_shape.setSize(m_buttonSize);
				m_shape.setPosition(m_buttonPosition);
				m_shape.setFillColor(m_isButtonHighlighted ? sf::Color::White : sf::Color::Black);
				renderTarget.draw(m_shape);

				m_text.setPosition(m_shape.getPosition().x + 15, m_shape.getPosition().y + 15 - m_text.getLocalBounds().top);
				renderTarget.draw(m_text);
			}
		}

		void Show(std::string message, std::function<void()> confirmClickedCallback) {
			m_isVisible = true;
			m_textString = message;

			m_text.setCharacterSize(30);
			m_text.setString("Play again");

			m_buttonSize.x = m_text.getLocalBounds().width + 30;
			m_buttonSize.y = m_text.getLocalBounds().height + 30;
			m_buttonPosition.x = m_position.x + (m_size.x - m_buttonSize.x) / 2;
			m_buttonPosition.y = m_position.y + m_size.y - m_buttonSize.y - 30;

			m_confirmClickedCallback = confirmClickedCallback;
		}

		void Close() {
			m_isVisible = false;
		}

		bool IsVisible() const {
			return m_isVisible;
		}

		void SetFont(sf::Font& font) {
			m_text.setFont(font);
		}

		void SetPosition(float x, float y) {
			m_position.x = x;
			m_position.y = y;
		}

		void SetSize(float width, float height) {
			m_size.x = width;
			m_size.y = height;
		}
	};
}

#endif // ___GameClones_ModalWindow_h___
