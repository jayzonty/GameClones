#pragma once

#ifndef ___Common_RectangleShape_h___
#define ___Common_RectangleShape_h___

#include "Drawable.h"
#include "Vertex.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "Rect2D.h"
#include "Matrix.h"

#include <GL/glew.h>

namespace Common {
	class RectangleShape : public Drawable {
		Vertex m_vertices[6];
		Rect2D m_rect;

		GLuint m_vbo, m_vao;
		Color m_color;

		Matrix m_transformationMatrix;

		ShaderProgram m_shaderProgram;

		bool m_isDirty;

	public:
		RectangleShape(const float& x, 
			const float& y, 
			const float& width, 
			const float& height, 
			const Color& color = Color::White) : Drawable(), m_rect(x, y, width, height), m_vbo(0), m_vao(0), m_color(color), m_isDirty(true) {
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::x)));
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Common::Vertex), reinterpret_cast<void*>(offsetof(Common::Vertex, Common::Vertex::r)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);

			if (!m_shaderProgram.Load("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag")) {
				std::printf("%s\n", m_shaderProgram.GetLastErrorMessage().c_str());
			}
		}

		RectangleShape() : RectangleShape(0.0f, 0.0f, 0.0f, 0.0f) {
		}

		~RectangleShape() {
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		const Color& GetColor() const {
			return m_color;
		}

		virtual void SetColor(const Color& color) {
			m_color = color;
			m_isDirty = true;
		}

		void SetPosition(float x, float y) {
			m_rect.x = x;
			m_rect.y = y;
			m_isDirty = true;
		}

		void SetSize(float width, float height) {
			m_rect.width = width;
			m_rect.height = height;
			m_isDirty = true;
		}

		const Rect2D& GetRect() const {
			return m_rect;
		}

		void Draw(const Matrix& transformationMatrix) {
			if (m_isDirty) {
				UpdateVertices();
				m_isDirty = false;
			}

			glBindVertexArray(m_vao);

			m_shaderProgram.Use();
			m_shaderProgram.SetUniformMatrix4f("projectionViewModelMatrix", (transformationMatrix * m_transformationMatrix).mat);
			
			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_shaderProgram.Unuse();

			glBindVertexArray(0);
		}

	private:
		void UpdateVertices() {
			const Color& color = GetColor();
			m_vertices[0] = { m_rect.x, m_rect.y, 0.0f, color.r, color.g, color.b, color.a };
			m_vertices[1] = { m_rect.x + m_rect.width, m_rect.y, 0.0f, color.r, color.g, color.b, color.a };
			m_vertices[2] = { m_rect.x + m_rect.width, m_rect.y + m_rect.height, 0.0f, color.r, color.g, color.b, color.a };
			m_vertices[3] = { m_rect.x + m_rect.width, m_rect.y + m_rect.height, 0.0f, color.r, color.g, color.b, color.a };
			m_vertices[4] = { m_rect.x, m_rect.y + m_rect.height, 0.0f, color.r, color.g, color.b, color.a };
			m_vertices[5] = { m_rect.x, m_rect.y, 0.0f, color.r, color.g, color.b, color.a };

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(Vertex), m_vertices);
		}
	};
}

#endif // ___Common_RectangleShape_h___
