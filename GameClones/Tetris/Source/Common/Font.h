#pragma once

#ifndef ___Common_Font_h___
#define ___Common_Font_h___

#include "GLIncludes.h"

#include <vector>
#include <string>

namespace Common {
	class Font {
	public:
		struct GlyphMetrics {
			unsigned int width;
			unsigned int height;

			int bearingX;
			int bearingY;

			int advance;
		};

	private:
		friend class Text;

		int m_fontSize;
		std::vector<GlyphMetrics> m_glyphMetrics;

		GLuint m_glyphTextures[128];

		bool m_isLoaded;

		std::string m_fontFilePath;

	public:
		Font();
		Font(const Font& other);

		~Font();

		bool Load(const char* fontFilePath);
		bool IsLoaded() const;

		void SetSize(int fontSize);

		const GlyphMetrics* GetGlyphMetrics(char c);

	private:
		GLuint GetGlyphTextureId(char c);
	};
}

#endif // ___Common_Font_h___
