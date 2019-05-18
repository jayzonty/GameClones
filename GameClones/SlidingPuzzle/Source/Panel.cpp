#include "Panel.h"

#include "Common/Rect.h"

namespace GameClones {
	namespace SlidingPuzzle {
		Panel::Panel(int id) : m_id(id) {
			m_text.SetColor(Common::Color::White);
			m_text.SetString(std::to_string(id));

			m_shape.SetFillColor(Common::Color(0, 150, 100, 255));
		}

		Panel::~Panel() {
		}

		int Panel::GetId() const {
			return m_id;
		}

		void Panel::Draw(Common::RenderTarget& renderTarget) {
			if (m_id > 0) {
				renderTarget.Draw(m_shape);
				renderTarget.Draw(m_text);
			}
		}

		void Panel::SetPosition(int x, int y) {
			m_shape.SetPosition(x, y);
			UpdateTextPosition();
		}

		void Panel::SetSize(unsigned int width, unsigned int height) {
			m_shape.SetSize(width, height);
			UpdateTextPosition();
		}

		void Panel::SetFont(Common::Font& font) {
			m_text.SetFont(font);
		}

		void Panel::UpdateTextPosition() {
			unsigned int textWidth, textHeight;
			m_text.ComputeSize(&textWidth, &textHeight);

			Common::Rect<int> rect = m_shape.GetRect();
			m_text.SetPosition(
				rect.x + (rect.width - textWidth) / 2, 
				rect.y + (rect.height - textHeight) / 2);
		}
	}
}
