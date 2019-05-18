#include "RectangleShape.h"

#include "MatrixTransform.h"
#include "MathUtils.h"

namespace Common {
	RectangleShape::RectangleShape(int x, int y,
		unsigned int width, unsigned int height,
		const Color& color) : Drawable(), m_rect(x, y, width, height), m_vbo(0), m_vao(0), m_fillColor(color), m_borderColor(Color::Black), m_borderSize(0) {
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		Vertex vertices[6] = {
			{ 0.0f, 0.0f, 0.0f, 255, 255, 255, 255 },
			{ 1.0f, 0.0f, 0.0f, 255, 255, 255, 255 },
			{ 1.0f, 1.0f, 0.0f, 255, 255, 255, 255 },
			{ 1.0f, 1.0f, 0.0f, 255, 255, 255, 255 },
			{ 0.0f, 1.0f, 0.0f, 255, 255, 255, 255 },
			{ 0.0f, 0.0f, 0.0f, 255, 255, 255, 255 }
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

	RectangleShape::RectangleShape() : RectangleShape(0, 0, 0, 0) {
	}

	RectangleShape::~RectangleShape() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	const Color& RectangleShape::GetFillColor() const {
		return m_fillColor;
	}

	void RectangleShape::SetFillColor(const Color& color) {
		m_fillColor = color;
	}

	const Color& RectangleShape::GetBorderColor() const {
		return m_borderColor;
	}

	void RectangleShape::SetBorderColor(const Color& color) {
		m_borderColor = color;
	}

	int RectangleShape::GetBorderSize() const {
		return m_borderSize;
	}

	void RectangleShape::SetBorderSize(int borderSize) {
		m_borderSize = borderSize;
		UpdateTransform();
	}

	void RectangleShape::SetPosition(int x, int y) {
		m_rect.x = x;
		m_rect.y = y;
		UpdateTransform();
	}

	void RectangleShape::SetSize(unsigned int width, unsigned int height) {
		m_rect.width = width;
		m_rect.height = height;
		UpdateTransform();
	}

	const Rect<int>& RectangleShape::GetRect() const {
		return m_rect;
	}
	void RectangleShape::Draw(const Matrix& transformationMatrix) {
		glBindVertexArray(m_vao);

		m_shaderProgram.Use();

		// Draw border
		// For now, just draw the border first, then have the fill color
		// overwrite the inner pixels.
		// In the future, should do a stencil test to draw the border to handle transparent fill color but with border
		m_shaderProgram.SetUniformMatrix4f("mvp", (transformationMatrix * m_borderTransform).mat);
		m_shaderProgram.SetUniform4f("color",
			m_borderColor.r / 255.0f, m_borderColor.g / 255.0f,
			m_borderColor.b / 255.0f, m_borderColor.a / 255.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Draw inner rect
		m_shaderProgram.SetUniformMatrix4f("mvp", (transformationMatrix * m_transform).mat);
		m_shaderProgram.SetUniform4f("color", 
			m_fillColor.r / 255.0f, m_fillColor.g / 255.0f,
			m_fillColor.b / 255.0f, m_fillColor.a / 255.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_shaderProgram.Unuse();

		glBindVertexArray(0);
	}

	void RectangleShape::UpdateTransform() {
		// Inner rect transform
		m_transform =
			MatrixTransform::CreateTranslation(m_rect.x + m_borderSize * 1.0f, m_rect.y + m_borderSize * 1.0f, 0.0f) *
			MatrixTransform::CreateScale(
				MathUtils::Max(0.0f, m_rect.width - m_borderSize * 2.0f), 
				MathUtils::Max(0.0f, m_rect.height - m_borderSize * 2.0f),
				1.0f);

		// Border transform
		m_borderTransform =
			MatrixTransform::CreateTranslation(m_rect.x * 1.0f, m_rect.y * 1.0f, 0.0f) *
			MatrixTransform::CreateScale(m_rect.width * 1.0f, m_rect.height * 1.0f, 1.0f);
	}

}