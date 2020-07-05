#pragma once

#include "Color.h"
#include "Drawable.h"
#include "Texture.h"
#include "GLIncludes.h"
#include "Matrix.h"
#include "Rect.h"
#include "ShaderProgram.h"
#include "Vector.h"

namespace Common
{
	class Sprite : public Drawable
	{
	private:
		Color m_color;

		Rect<float> m_rect;
		Rect<float> m_uvRect;

		Matrix m_transform;

		GLuint m_vbo, m_vao;
		ShaderProgram m_shaderProgram;

		Texture* m_texture;

	public:
		Sprite(Texture* texture);
		Sprite(Texture* texture, const Rect<float>& uvRect);
		~Sprite();

		Texture* GetTexture() const;

		void SetPosition(float x, float y);
		void SetSize(float width, float height);
		void SetColor(const Color& color);

		const Rect<float>& GetRect() const;
		const Rect<float> GetUVRect() const;

		void Draw(const Matrix& transformationMatrix) override;

	private:
		void UpdateTransform();
	};
}