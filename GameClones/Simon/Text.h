#pragma once

#ifndef ___Common_Text_h___
#define ___Common_Text_h___

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdio>

#include <GL/glew.h>

#include "Vector.h"
#include "Color.h"
#include "ShaderProgram.h"

#include <vector>

namespace Common {
	// Reference: https://learnopengl.com/In-Practice/Text-Rendering
	class Text {
		struct Character {
			GLuint textureId;
			Vector2 size;
			Vector2 bearing;
			GLuint advance;
		};

		int m_fontSize;
		bool m_isLoaded;
		std::string m_fontFilePath;

		std::vector<Character> m_characters;

		GLuint m_vbo, m_vao;
	public:
		Text() : m_fontSize(30), m_isLoaded(false), m_vbo(0), m_vao(0) {
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);

			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		~Text() {
			for (size_t i = 0; i < m_characters.size(); ++i) {
				glDeleteTextures(1, &m_characters[i].textureId);
			}

			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		bool Load(const char* fontFilePath) {
			m_isLoaded = false;

			FT_Library ftLibray;
			FT_Face ftFace;

			// FT libraries return 0 on success
			if (FT_Init_FreeType(&ftLibray)) {
				std::printf("Failed to load freetype library!\n");
			}
			else {
				m_isLoaded = true;
			}

			if (FT_New_Face(ftLibray, fontFilePath, 0, &ftFace)) {
				std::printf("Failed to load font! %s\n", fontFilePath);
			}
			else {
				m_fontFilePath = std::string(fontFilePath);

				m_characters.clear();

				FT_Set_Pixel_Sizes(ftFace, 0, m_fontSize);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				for (unsigned char c = 0; c < 128; ++c) {
					if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER)) {
						std::printf("Failed to load char %c\n", c);
						continue;
					}

					GLuint texture;
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
						ftFace->glyph->bitmap.width,
						ftFace->glyph->bitmap.rows,
						0, GL_RED, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);

					Character character = {
						texture,
						Vector2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
						Vector2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
						ftFace->glyph->advance.x
					};
					m_characters.push_back(character);
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				FT_Done_Face(ftFace);
				FT_Done_FreeType(ftLibray);

				m_isLoaded = true;
			}

			return m_isLoaded;
		}

		void SetFontSize(const int fontSize) {
			if (!m_isLoaded) {
				return;
			}

			m_fontSize = fontSize;
			Load(m_fontFilePath.c_str());
		}

		bool IsLoaded() const {
			return m_isLoaded;
		}

		void DrawString(const std::string& str, float x, float y, const Color& color, ShaderProgram& shader) {
			shader.SetUniform3f("textColor", color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

			glBindVertexArray(m_vao);
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (size_t i = 0; i < str.size(); ++i) {
				unsigned char c = str[i];
				Character ch = m_characters[c];

				float xPos = x + ch.bearing.x;
				float yPos = y - (ch.size.y - ch.bearing.y);

				float width = ch.size.x;
				float height = ch.size.y;

				GLfloat vertices[6][4] = 
				{
					{xPos, yPos, 0.0f, 1.0f},
					{xPos + width, yPos, 1.0f, 1.0f},
					{xPos + width, yPos + height, 1.0f, 0.0f},

					{xPos + width, yPos + height, 1.0f, 0.0f},
					{xPos, yPos + height, 0.0f, 0.0f},
					{xPos, yPos, 0.0f, 1.0f}
				};

				glBindTexture(GL_TEXTURE_2D, ch.textureId);

				glActiveTexture(GL_TEXTURE0);
				shader.SetUniform1i("tex", 0);

				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				x += (ch.advance >> 6);
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};
}

#endif // ___Common_Text_h___
