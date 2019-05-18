#pragma once

#ifndef ___Common_RectangleShape_h___
#define ___Common_RectangleShape_h___

#include "Drawable.h"
#include "Vertex.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "Rect.h"
#include "Matrix.h"

#include "GLIncludes.h"

namespace Common {
	class RectangleShape : public Drawable {
		Rect<int> m_rect;

		GLuint m_vbo, m_vao;

		Color m_fillColor;
		Color m_borderColor;

		Matrix m_transform;
		Matrix m_borderTransform;

		ShaderProgram m_shaderProgram;
		int m_borderSize;

	public:
		RectangleShape(int x, int y,
			unsigned int width, unsigned int height,
			const Color& color = Color::White);

		RectangleShape();
		~RectangleShape();

		const Color& GetFillColor() const;
		void SetFillColor(const Color& color);

		const Color& GetBorderColor() const;
		void SetBorderColor(const Color& color);

		int GetBorderSize() const;
		void SetBorderSize(int borderSize);

		void SetPosition(int x, int y);

		void SetSize(unsigned int width, unsigned int height);

		const Rect<int>& GetRect() const;

		void Draw(const Matrix& transformationMatrix) override;

	private:
		void UpdateTransform();
	};
}

#endif // ___Common_RectangleShape_h___