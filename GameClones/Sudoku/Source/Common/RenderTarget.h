#ifndef ___Common_RenderTarget_h___
#define ___Common_RenderTarget_h___

#include "Drawable.h"
#include "Matrix.h"
#include "Rect.h"

namespace Common
{
	class RenderTarget
	{
		Matrix m_projectionMatrix;
		unsigned int m_width;
		unsigned int m_height;

	public:
		RenderTarget();
		~RenderTarget();
		
		void SetSize(unsigned int width, unsigned int height);

		void Draw(Drawable& drawable);
	};
}

#endif // ___Common_RenderTarget_h___
