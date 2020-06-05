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

namespace Common
{
	class Text : public Drawable
	{
	public:
		enum class HorizontalAlignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

	private:
		struct Line {
			std::string str;
			unsigned int width;
		};

		GLuint m_vbo, m_vao;

		Font* m_font;

		ShaderProgram m_shader;
		Color m_color;
		std::vector<Line> m_lines;

		float m_lineSpacing;

		unsigned int m_lineHeight;

		float m_x, m_y;

		HorizontalAlignment m_horizontalAlign;

		unsigned int m_width, m_height;

	public:
		Text();
		~Text();

		void SetLineSpacing(float lineSpacing);

		void SetFont(Font& font);

		void SetString(const std::string& str);

		void SetColor(const Color& color);

		void SetPosition(float x, float y);
		Vector2 GetPosition() const;

		void Draw(const Matrix& transformationMatrix) override;

		void ComputeSize(unsigned int* width, unsigned int* height);

		void SetHorizontalAlignment(HorizontalAlignment horizontalAlign);
	};
}

#endif // ___Common_Text_h___
