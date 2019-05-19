#include "Button.h"

#include "Color.h"

namespace Common {
	Button::Button() : m_defaultBackgroundColor(255), 
		m_hoverBackgroundColor(Color::Blue),
		m_defaultTextColor(Color::Black), 
		m_hoverTextColor(Color::White) {
		m_shape.SetFillColor(m_defaultBackgroundColor);
		m_text.SetColor(m_defaultTextColor);
	}

	Button::~Button() {
	}

	void Button::SetPosition(float x, float y) {
		m_shape.SetPosition(x, y);
		UpdateTextPosition();
	}

	void Button::SetSize(float width, float height) {
		m_shape.SetSize(width, height);
		UpdateTextPosition();
	}

	void Button::SetText(const std::string& text) {
		m_text.SetString(text);
		UpdateTextPosition();
	}

	void Button::SetFont(Font& font) {
		m_text.SetFont(font);
	}

	void Button::SetBackgroundColor(const Color& color) {
		m_defaultBackgroundColor = color;
	}

	void Button::SetHoverBackgroundColor(const Color& color) {
		m_hoverBackgroundColor = color;
	}

	void Button::SetTextColor(const Color& color) {
		m_defaultTextColor = color;
	}

	void Button::SetHoverTextColor(const Color& color) {
		m_hoverTextColor = color;
	}

	void Button::AddClickListener(std::function<void(void)> listener) {
		m_clickListeners.push_back(listener);
	}

	void Button::Click() {
		for (size_t i = 0; i < m_clickListeners.size(); ++i) {
			m_clickListeners[i]();
		}
	}

	void Button::Hover() {
		m_shape.SetFillColor(m_hoverBackgroundColor);
		m_text.SetColor(m_hoverTextColor);
	}

	void Button::Unhover() {
		m_shape.SetFillColor(m_defaultBackgroundColor);
		m_text.SetColor(m_defaultTextColor);
	}

	const Rect<float>& Button::GetBounds() const {
		return m_shape.GetRect();
	}

	void Button::Draw(const Matrix& transformationMatrix) {
		m_shape.Draw(transformationMatrix);
		m_text.Draw(transformationMatrix);
	}

	void Button::UpdateTextPosition() {
		unsigned int textWidth, textHeight;
		m_text.ComputeSize(&textWidth, &textHeight);
		m_text.SetPosition(
			m_shape.GetRect().x + (m_shape.GetRect().width - textWidth) / 2,
			m_shape.GetRect().y + (m_shape.GetRect().height - textHeight) / 2);
	}
}