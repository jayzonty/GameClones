#include "RectangleShape.h"

#include "MatrixTransform.h"
#include "Math.h"

namespace Common
{
	RectangleShape::RectangleShape(float x, float y,
		float width, float height,
		const Color& color) : Drawable(), m_rect(x, y, width, height), 
		m_vbo(0), m_vao(0), m_fillColor(color), m_borderColor(Color::Black), 
		m_leftBorderSize(0.0f), m_rightBorderSize(0.0f), 
		m_topBorderSize(0.0f), m_bottomBorderSize(0.0f)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		Vertex vertices[6] =
		{
			{ 0.0f, 0.0f, 0.0f, 255, 255, 255, 255 }
			, { 1.0f, 0.0f, 0.0f, 255, 255, 255, 255 }
			, { 1.0f, 1.0f, 0.0f, 255, 255, 255, 255 }
			, { 1.0f, 1.0f, 0.0f, 255, 255, 255, 255 }
			, { 0.0f, 1.0f, 0.0f, 255, 255, 255, 255 }
			, { 0.0f, 0.0f, 0.0f, 255, 255, 255, 255 }
		};

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::x)));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::r)));

		glBindVertexArray(0);

		if (!m_shaderProgram.Load("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag")) {
			std::printf("%s\n", m_shaderProgram.GetLastErrorMessage().c_str());
		}
	}

	RectangleShape::RectangleShape() : RectangleShape(0, 0, 0, 0)
	{
	}

	RectangleShape::~RectangleShape()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	const Color& RectangleShape::GetFillColor() const
	{
		return m_fillColor;
	}

	void RectangleShape::SetFillColor(const Color& color)
	{
		m_fillColor = color;
	}

	const Color& RectangleShape::GetBorderColor() const
	{
		return m_borderColor;
	}

	void RectangleShape::SetBorderColor(const Color& color)
	{
		m_borderColor = color;
	}

	void RectangleShape::SetBorderSizes(float left, float right, float top, float bottom)
	{
		m_leftBorderSize = left;
		m_rightBorderSize = right;
		m_topBorderSize = top;
		m_bottomBorderSize = bottom;
		UpdateTransform();
	}

	void RectangleShape::GetBorderSizes(float* left, float* right, float* top, float* bottom)
	{
		*left = m_leftBorderSize;
		*right = m_rightBorderSize;
		*top = m_topBorderSize;
		*bottom = m_bottomBorderSize;
	}

	void RectangleShape::SetPosition(float x, float y)
	{
		m_rect.x = x;
		m_rect.y = y;
		UpdateTransform();
	}

	void RectangleShape::SetSize(float width, float height)
	{
		m_rect.width = width;
		m_rect.height = height;
		UpdateTransform();
	}

	const Rect<float>& RectangleShape::GetRect() const
	{
		return m_rect;
	}
	
	void RectangleShape::Draw(const Matrix& transformationMatrix)
	{
		glClear(GL_STENCIL_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glBindVertexArray(m_vao);

		m_shaderProgram.Use();

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Fragments always write to stencil buffer
		glStencilMask(0xFF); // Enable writing to stencil buffer

		// Draw inner rect
		m_shaderProgram.SetUniformMatrix4f("mvp", (transformationMatrix * m_transform).mat);
		m_shaderProgram.SetUniform4f("color",
			m_fillColor.r / 255.0f, m_fillColor.g / 255.0f,
			m_fillColor.b / 255.0f, m_fillColor.a / 255.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Only draw on the parts where the stencil value is 1
		glStencilMask(0x00); // Disable writing to stencil buffer

		// Draw border
		// For now, just draw the border first, then have the fill color
		// overwrite the inner pixels.
		// In the future, should do a stencil test to draw the border to handle transparent fill color but with border
		m_shaderProgram.SetUniformMatrix4f("mvp", (transformationMatrix * m_borderTransform).mat);
		m_shaderProgram.SetUniform4f("color",
			m_borderColor.r / 255.0f, m_borderColor.g / 255.0f,
			m_borderColor.b / 255.0f, m_borderColor.a / 255.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glStencilMask(0xFF); // Revert back to being able to write to stencil buffer

		m_shaderProgram.Unuse();

		glBindVertexArray(0);

		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
	}

	void RectangleShape::UpdateTransform()
	{
		// Inner rect transform
		m_transform =
			MatrixTransform::CreateTranslation(m_rect.x + m_leftBorderSize, m_rect.y + m_bottomBorderSize, 0.0f) *
			MatrixTransform::CreateScale(
				Math::Max(0.0f, m_rect.width - m_rightBorderSize - m_leftBorderSize),
				Math::Max(0.0f, m_rect.height - m_topBorderSize - m_bottomBorderSize),
				1.0f);

		// Border transform
		m_borderTransform =
			MatrixTransform::CreateTranslation(m_rect.x, m_rect.y, 0.0f) *
			MatrixTransform::CreateScale(m_rect.width, m_rect.height, 1.0f);
	}

}
