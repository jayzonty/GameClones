#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Common
{
	Texture::Texture()
		: m_data(nullptr)
		, m_width(0)
		, m_height(0)
		, m_numChannels(0)
		, m_textureHandle(0)
	{
	}

	Texture::~Texture()
	{
		if (m_data != nullptr)
		{
			stbi_image_free(m_data);
			m_data = nullptr;
		}

		if (m_textureHandle != 0)
		{
			glDeleteTextures(1, &m_textureHandle);
			m_textureHandle = 0;
		}
	}

	GLuint Texture::GetHandle() const
	{
		return m_textureHandle;
	}

	int Texture::GetWidth() const
	{
		return m_width;
	}

	int Texture::GetHeight() const
	{
		return m_height;
	}

	bool Texture::Load(const char* imageFilePath)
	{
		stbi_set_flip_vertically_on_load(true);
		m_data = stbi_load(imageFilePath, &m_width, &m_height, &m_numChannels, 0);

		if (m_data == nullptr)
		{
			return false;
		}

		if (m_textureHandle == 0)
		{
			glGenTextures(1, &m_textureHandle);
		}

		glBindTexture(GL_TEXTURE_2D, m_textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_numChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
		}
		else if (m_numChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		return true;
	}
}
