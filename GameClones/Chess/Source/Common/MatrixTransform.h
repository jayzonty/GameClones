#ifndef ___Common_MatrixTransform_h___
#define ___Common_MatrixTransform_h___

#include "Matrix.h"
#include "Vector.h"

namespace Common
{
	namespace MatrixTransform
	{
		Matrix CreateTranslation(float tx, float ty, float tz);

		Matrix CreateScale(float sx, float sy, float sz);

		Matrix CreateXRotation(float theta, bool isDegrees);

		Matrix CreateYRotation(float theta, bool isDegrees);

		Matrix CreateZRotation(float theta, bool isDegrees);

		Matrix CreateOrtho(float left, float right, float bottom, float top, float near, float far);

		Matrix CreateOrtho(float left, float right, float bottom, float top);

		Matrix CreatePerspective(float fieldOfViewY, float width, float height, float near, float far);

		Matrix LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
	}
}

#endif // ___Common_MatrixTransform_h___
