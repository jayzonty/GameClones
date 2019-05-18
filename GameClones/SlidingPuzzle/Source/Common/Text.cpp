#include "Text.h"

#include "MathUtils.h"

namespace Common {
	Text::Text() : m_vbo(0), m_vao(0), m_font(nullptr), m_color(0), m_x(0), m_y(0) {
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);

		glBindVertexArray(0);

		m_shader.Load("Resources/Shaders/Font.vert", "Resources/Shaders/Font.frag");
	}

	Text::~Text() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Text::SetFont(Font& font) {
		m_font = &font;
	}

	void Text::SetString(const std::string& str) {
		m_string = str;
	}
	
	void Text::SetColor(const Color& color) {
		m_color = color;
	}

	void Text::SetPosition(int x, int y) {
		m_x = x;
		m_y = y;
	}

	void Text::Draw(const Matrix& transformationMatrix) {
		if ((nullptr == m_font) || !m_font->IsLoaded()) {
			return;
		}

		m_shader.Use();

		m_shader.SetUniformMatrix4f("projection", transformationMatrix.mat);
		m_shader.SetUniform3f("textColor", 
			m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f);

		glBindVertexArray(m_vao);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int x = m_x;
		int y = m_y;
		for (size_t i = 0; i < m_string.size(); ++i) {
			char c = m_string[i];
			const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);
			if (nullptr == metrics) {
				continue;
			}

			float xPos = static_cast<float>(x + metrics->bearingX);
			float yPos = static_cast<float>(y - (metrics->height - metrics->bearingY));

			unsigned int width = metrics->width;
			unsigned int height = metrics->height;

			GLfloat vertices[6][4] =
			{
				{xPos, yPos, 0.0f, 1.0f},
				{xPos + width, yPos, 1.0f, 1.0f},
				{xPos + width, yPos + height, 1.0f, 0.0f},

				{xPos + width, yPos + height, 1.0f, 0.0f},
				{xPos, yPos + height, 0.0f, 0.0f},
				{xPos, yPos, 0.0f, 1.0f}
			};

			glBindTexture(GL_TEXTURE_2D, m_font->GetGlyphTextureId(c));

			glActiveTexture(GL_TEXTURE0);
			m_shader.SetUniform1i("tex", 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += metrics->advance;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_shader.Unuse();
	}

	void Text::ComputeSize(unsigned int* width, unsigned int* height) {
		unsigned int w = 0, h = 0;

		if (m_font->IsLoaded()) {
			for (size_t i = 0; i < m_string.size(); ++i) {
				char c = m_string[i];
				const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);

				w += metrics->advance;
				h = MathUtils::Max(h, metrics->height);
			}
		}

		if (nullptr != width) {
			*width = w;
		}
		if (nullptr != height) {
			*height = h;
		}
	}
}