#include "RenderTarget.h"

#include "MatrixTransform.h"

namespace Common {
	RenderTarget::RenderTarget() : m_width(0), m_height(0) {
	}

	RenderTarget::~RenderTarget() {
	}

	void RenderTarget::SetSize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;

		m_projectionMatrix = MatrixTransform::CreateOrtho(0.0f, m_width * 1.0f, 0.0f, m_height * 1.0f);
	}

	void RenderTarget::Draw(Drawable& drawable) {
		drawable.Draw(m_projectionMatrix);
	}
}