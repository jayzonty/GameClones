#include "Text.h"

#include "Math.h"

namespace Common
{
	Text::Text()
		: m_vbo(0)
		, m_vao(0)
		, m_font(nullptr)
		, m_color(0)
		, m_lineSpacing(0.0f)
		, m_lineHeight(0)
		, m_x(0)
		, m_y(0)
		, m_horizontalAlign(HorizontalAlignment::LEFT)
		, m_width(0)
		, m_height(0)
	{
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

	Text::~Text()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Text::SetLineSpacing(float lineSpacing)
	{
		m_lineSpacing = lineSpacing;
	}

	void Text::SetFont(Font& font)
	{
		m_font = &font;
	}

	void Text::SetString(const std::string& str)
	{
		m_lines.clear();

		m_lineHeight = 0;

		std::string line;
		unsigned int lineWidth = 0;
		for (char c : str)
		{
			if (c == '\n')
			{
				m_lines.emplace_back();
				m_lines.back().str = line;
				m_lines.back().width = lineWidth;

				line = "";
				lineWidth = 0;
			}
			else
			{
				line += c;
				const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);
				if (metrics != nullptr)
				{
					m_lineHeight = Common::Math::Max(m_lineHeight, metrics->height);
					lineWidth += metrics->advance;
				}
			}
		}

		if (!line.empty())
		{
			m_lines.emplace_back();
			m_lines.back().str = line;
			m_lines.back().width = lineWidth;
		}

		ComputeSize(&m_width, &m_height);
	}
	
	void Text::SetColor(const Color& color)
	{
		m_color = color;
	}

	void Text::SetPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	Vector2 Text::GetPosition() const
	{
		return Vector2(m_x, m_y);
	}

	void Text::Draw(const Matrix& transformationMatrix)
	{
		if ((nullptr == m_font) || !m_font->IsLoaded())
		{
			return;
		}

		m_shader.Use();

		m_shader.SetUniformMatrix4f("projection", transformationMatrix.mat);
		m_shader.SetUniform3f("textColor", 
			m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f);

		glBindVertexArray(m_vao);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float x = m_x;
		float y = m_y;
		for (int j = m_lines.size() - 1; j >= 0; --j)
		{
			if (j < m_lines.size() - 1)
			{
				y += m_lineHeight + m_lineSpacing;
			}

			Line& line = m_lines[j];

			if (m_horizontalAlign == HorizontalAlignment::LEFT)
			{
				x = m_x;
			}
			else if (m_horizontalAlign == HorizontalAlignment::CENTER)
			{
				x = m_x + (m_width - line.width) / 2.0f;
			}
			else
			{
				x = m_x + m_width - line.width;
			}

			std::string& lineStr = line.str;
			for (size_t i = 0; i < lineStr.size(); ++i)
			{
				char c = lineStr[i];
				const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);
				if (nullptr == metrics)
				{
					continue;
				}

				float xPos = static_cast<float>(x + metrics->bearingX);
				float yPos = static_cast<float>(y - (metrics->height - metrics->bearingY));

				unsigned int width = metrics->width;
				unsigned int height = metrics->height;

				GLfloat vertices[6][4] =
				{
					{ xPos, yPos, 0.0f, 1.0f },
					{ xPos + width, yPos, 1.0f, 1.0f },
					{ xPos + width, yPos + height, 1.0f, 0.0f },

					{ xPos + width, yPos + height, 1.0f, 0.0f },
					{ xPos, yPos + height, 0.0f, 0.0f },
					{ xPos, yPos, 0.0f, 1.0f }
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
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		m_shader.Unuse();
	}

	void Text::ComputeSize(unsigned int* width, unsigned int* height)
	{
		unsigned int w = 0, maxLineWidth = 0;

		if (m_font->IsLoaded())
		{
			for (size_t j = 0; j < m_lines.size(); ++j)
			{
				std::string& line = m_lines[j].str;
				for (size_t i = 0; i < line.size(); ++i)
				{
					char c = line[i];
					const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);

					w += metrics->advance;
				}

				maxLineWidth = Common::Math::Max(maxLineWidth, w);
				w = 0;
			}
		}

		if (nullptr != width)
		{
			*width = maxLineWidth;
		}
		if (nullptr != height)
		{
			*height = m_lines.size() * m_lineHeight + m_lineSpacing * (m_lines.size() - 1);
		}
	}

	void Text::SetHorizontalAlignment(HorizontalAlignment horizontalAlign)
	{
		m_horizontalAlign = horizontalAlign;
	}
}