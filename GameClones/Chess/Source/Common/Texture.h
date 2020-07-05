#pragma once

#include "GLIncludes.h"

namespace Common
{
	class Texture
	{
	private:
		unsigned char* m_data;
		int m_width, m_height, m_numChannels;
		GLuint m_textureHandle;

	public:
		Texture();
		~Texture();

		GLuint GetHandle() const;

		int GetWidth() const;
		int GetHeight() const;

		bool Load(const char* imageFilePath);
	};
}
