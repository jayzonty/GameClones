#pragma once

#ifndef ___Common_Button_h___
#define ___Common_Button_h___

#include <functional>
#include <vector>

#include "Drawable.h"
#include "Matrix.h"

#include "RectangleShape.h"
#include "Rect.h"
#include "Text.h"
#include "Font.h"
#include "Color.h"

namespace Common {
	class Button : public Drawable {
		std::vector< std::function<void(void)> > m_clickListeners;

		Common::RectangleShape m_shape;
		Common::Text m_text;

		Common::Color m_defaultBackgroundColor;
		Common::Color m_hoverBackgroundColor;

		Common::Color m_defaultTextColor;
		Common::Color m_hoverTextColor;

	public:
		Button();
		~Button();

		void SetPosition(int x, int y);
		void SetSize(unsigned int width, unsigned int height);

		void SetText(const std::string& text);
		void SetFont(Font& font);

		void SetBackgroundColor(const Color& color);
		void SetHoverBackgroundColor(const Color& color);

		void SetTextColor(const Color& color);
		void SetHoverTextColor(const Color& color);

		void AddClickListener(std::function<void(void)> listener);

		void Click();

		void Hover();
		void Unhover();

		const Rect<int>& GetBounds() const;

		void Draw(const Matrix& transformationMatrix) override;

	private:
		void UpdateTextPosition();
	};
}

#endif // ___Common_Button_h___
