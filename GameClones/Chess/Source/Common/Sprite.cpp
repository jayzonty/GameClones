#include "Sprite.h"

#include "MatrixTransform.h"
#include "Vertex.h"

namespace Common
{
	Sprite::Sprite(Texture* texture)
		: Sprite(texture, Rect<float>(0.0f, 0.0f, 1.0f, 1.0f))
	{
	}

	Sprite::Sprite(Texture* texture, const Rect<float>& uvRect)
		: Drawable()
		, m_color()
		, m_rect()
		, m_uvRect(uvRect)
		, m_transform()
		, m_vbo(0)
		, m_vao(0)
		, m_shaderProgram()
		, m_texture(texture)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		Vertex vertices[6] =
		{
			{ 0.0f, 0.0f, 0.0f, m_uvRect.x, m_uvRect.y, 255, 255, 255, 255 }
			, { 1.0f, 0.0f, 0.0f, m_uvRect.x + m_uvRect.width, m_uvRect.y, 255, 255, 255, 255 }
			, { 1.0f, 1.0f, 0.0f, m_uvRect.x + m_uvRect.width, m_uvRect.y + m_uvRect.height, 255, 255, 255, 255 }
			, { 1.0f, 1.0f, 0.0f, m_uvRect.x + m_uvRect.width, m_uvRect.y + m_uvRect.height, 255, 255, 255, 255 }
			, { 0.0f, 1.0f, 0.0f, m_uvRect.x, m_uvRect.y + m_uvRect.height, 255, 255, 255, 255 }
			, { 0.0f, 0.0f, 0.0f, m_uvRect.x, m_uvRect.y, 255, 255, 255, 255 }
		};

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::x)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::u)));

		glBindVertexArray(0);

		if (!m_shaderProgram.Load("Resources/Shaders/Sprite.vert", "Resources/Shaders/Sprite.frag")) {
			std::printf("%s\n", m_shaderProgram.GetLastErrorMessage().c_str());
		}
	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	Texture* Sprite::GetTexture() const
	{
		return m_texture;
	}

	void Sprite::SetPosition(float x, float y)
	{
		m_rect.x = x;
		m_rect.y = y;
		UpdateTransform();
	}

	void Sprite::SetSize(float width, float height)
	{
		m_rect.width = width;
		m_rect.height = height;
		UpdateTransform();
	}

	void Sprite::SetColor(const Color& color)
	{
		m_color = color;
	}

	const Rect<float>& Sprite::GetRect() const
	{
		return m_rect;
	}

	const Rect<float> Sprite::GetUVRect() const
	{
		return m_uvRect;
	}

	void Sprite::Draw(const Matrix & transformationMatrix)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetHandle());

		glBindVertexArray(m_vao);

		m_shaderProgram.Use();

		// Draw sprite rect
		m_shaderProgram.SetUniformMatrix4f("mvp", (transformationMatrix * m_transform).mat);
		m_shaderProgram.SetUniform4f("color",
			m_color.r / 255.0f, m_color.g / 255.0f,
			m_color.b / 255.0f, m_color.a / 255.0f);
		m_shaderProgram.SetUniform1i("tex", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_shaderProgram.Unuse();

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}

	void Sprite::UpdateTransform()
	{
		m_transform =
			MatrixTransform::CreateTranslation(m_rect.x, m_rect.y, 0.0f) *
			MatrixTransform::CreateScale(m_rect.width, m_rect.height, 1.0f);
	}
}
