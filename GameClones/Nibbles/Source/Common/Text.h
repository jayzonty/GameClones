#pragma once

#ifndef ___Common_Text_h___
#define ___Common_Text_h___

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdio>

#include "GLIncludes.h"

#include "Color.h"
#include "ShaderProgram.h"
#include "Drawable.h"
#include "Matrix.h"
#include "Vector.h"

#include "Font.h"

#include <vector>

namespace Common {
	class Text : public Drawable {
		GLuint m_vbo, m_vao;

		Font* m_font;

		ShaderProgram m_shader;
		Color m_color;
		std::string m_string;
		float m_x, m_y;

	public:
		Text();
		~Text();

		void SetFont(Font& font);

		void SetString(const std::string& str);

		void SetColor(const Color& color);

		void SetPosition(float x, float y);
		Vector2 GetPosition() const;

		void Draw(const Matrix& transformationMatrix) override;

		void ComputeSize(unsigned int* width, unsigned int* height);
	};
}

#endif // ___Common_Text_h___
