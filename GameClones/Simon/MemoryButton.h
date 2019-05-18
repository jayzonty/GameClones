#pragma once

#ifndef ___GameClones_Simon_MemoryButton_h___
#define ___GameClones_Simon_MemoryButton_h___

#include "Drawable.h"
#include "Matrix.h"
#include "Timer.h"
#include "Color.h"
#include "RectangleShape.h"

namespace GameClones {
	namespace Simon {
		class MemoryButton : public Common::Drawable {
			int m_id;

			Common::RectangleShape m_shape;

			float m_highlightDuration;
			float m_highlightInterval;
			int m_blinkCount;

			Common::Timer m_timer;
			int m_blinksDone;
			bool m_isBlinking;

			Common::Color m_defaultColor;
			Common::Color m_highlightColor;
			bool m_isHighlighted;
		public:
			MemoryButton(int id, float x, float y,
				float width, float height, 
				const Common::Color& defaultColor = Common::Color::White,
				const Common::Color& highlightColor = Common::Color::Yellow)
				: m_id(id), m_shape(x, y, width, height), m_highlightDuration(0.15f), 
				m_highlightInterval(0.15f), m_blinkCount(2),
				m_defaultColor(defaultColor), m_highlightColor(highlightColor),
				m_blinksDone(0), m_isBlinking(false), m_isHighlighted(false) {
				m_shape.SetFillColor(defaultColor);
			}

			MemoryButton(int id) : MemoryButton(id, 0.0f, 0.0f, 0.0f, 0.0f) {
			}

			void SetHighlightDuration(float duration) {
				m_highlightDuration = duration;
			}

			void SetHighlightInterval(float duration) {
				m_highlightInterval = duration;
			}

			void SetBlinkCount(int blinkCount) {
				m_blinkCount = blinkCount;
			}

			void SetPosition(float x, float y) {
				m_shape.SetPosition(x, y);
			}

			void SetSize(float width, float height) {
				m_shape.SetSize(width, height);
			}
			
			void SetDefaultColor(const Common::Color& color) {
				m_defaultColor = color;
			}

			void SetHighlightColor(const Common::Color& color) {
				m_highlightColor = color;
			}

			const Common::Rect2D& GetRect() const {
				return m_shape.GetRect();
			}

			void Update(float deltaTime) {
				if (!m_isBlinking) {
					return;
				}

				m_timer.Update(deltaTime);

				if (m_isHighlighted && m_timer.IsFinished()) {
					m_isHighlighted = false;
					m_shape.SetFillColor(m_defaultColor);

					++m_blinksDone;
					if (m_blinksDone < m_blinkCount) {
						m_timer.SetDuration(m_highlightInterval);
						m_timer.Start();
					}
					else {
						m_isBlinking = false;
					}
				}
				else if (m_timer.IsFinished()) {
					m_isHighlighted = true;
					m_shape.SetFillColor(m_highlightColor);

					m_timer.SetDuration(m_highlightDuration);
					m_timer.Start();
				}
			}

			void Draw(const Common::Matrix& transformationMatrix) override {
				m_shape.Draw(transformationMatrix);
			}

			void Blink(int numBlinks = 1) {
				m_isBlinking = true;

				m_shape.SetFillColor(m_highlightColor);
				m_isHighlighted = true;

				m_timer.SetDuration(m_highlightDuration);
				m_timer.Start();
				m_blinksDone = 0;
				m_blinkCount = numBlinks;
			}

			int GetId() const {
				return m_id;
			}
		};
	}
}

#endif // ___GameClones_Simon_MemoryButton_h___
