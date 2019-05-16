#pragma once

#ifndef ___Common_MatrixTransform_h___
#define ___Common_MatrixTransform_h___

#include "Matrix.h"
#include "MathUtils.h"
#include "Vector.h"

#include <cmath>

namespace Common {
	namespace MatrixTransform {
		inline Matrix CreateTranslation(float tx, float ty, float tz) {
			Matrix ret;
			ret[12] = tx;
			ret[13] = ty;
			ret[14] = tz;

			return ret;
		}

		inline Matrix CreateScale(float sx, float sy, float sz) {
			Matrix ret;
			ret[0] = sx;
			ret[5] = sy;
			ret[10] = sz;

			return ret;
		}

		inline Matrix CreateXRotation(float theta, bool isDegrees) {
			if (isDegrees)
				theta = theta * MathUtils::PI_F / 180.0f;

			float c = cos(theta);
			float s = sin(theta);

			Matrix ret;
			ret[5] = c;
			ret[6] = s;
			ret[9] = -s;
			ret[10] = c;

			return ret;
		}

		inline Matrix CreateYRotation(float theta, bool isDegrees) {
			if (isDegrees)
				theta = theta * MathUtils::PI_F / 180.0f;

			float c = cos(theta);
			float s = sin(theta);

			Matrix ret;
			ret[0] = c;
			ret[2] = -s;
			ret[8] = s;
			ret[10] = c;

			return ret;
		}

		inline Matrix CreateZRotation(float theta, bool isDegrees) {
			if (isDegrees)
				theta = theta * MathUtils::PI_F / 180.0f;

			float c = cos(theta);
			float s = sin(theta);

			Matrix ret;
			ret[0] = c;
			ret[1] = s;
			ret[4] = -s;
			ret[5] = c;

			return ret;
		}

		inline Matrix CreateOrtho(float left, float right, float bottom, float top, float near, float far) {
			Matrix ret;

			ret[0] = 2 / (right - left); ret[4] = 0.0f; ret[8] = 0.0f; ret[12] = -(right + left) / (right - left);
			ret[1] = 0.0f; ret[5] = 2 / (top - bottom); ret[9] = 0.0f; ret[13] = -(top + bottom) / (top - bottom);
			ret[2] = 0.0f; ret[6] = 0.0f; ret[10] = -2 / (far - near); ret[14] = -(far + near) / (far - near);
			ret[3] = 0.0f; ret[7] = 0.0f; ret[11] = 0.0f; ret[15] = 1.0f;

			return ret;
		}

		inline Matrix CreateOrtho(float left, float right, float bottom, float top) {
			return CreateOrtho(left, right, bottom, top, 0.0f, -1.0f);
		}

		inline Matrix CreatePerspective(float fieldOfViewY, float width, float height, float near, float far) {
			Matrix ret;

			fieldOfViewY = fieldOfViewY * MathUtils::PI_F / 180.0f;

			float halfAngle = fieldOfViewY / 2.0f;
			float aspectRatio = width / height;

			ret[0] = (cos(halfAngle) / sin(halfAngle)) / aspectRatio;
			ret[5] = cos(halfAngle) / sin(halfAngle);
			ret[10] = -(far + near) / (far - near);
			ret[11] = -1.0f;
			ret[14] = -2 * far * near / (far - near);
			ret[15] = 0.0f;

			return ret;
		}

		inline Matrix LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {

			Vector3 z = eye - target;
			Vector3 x = Vector3::cross(up, z);
			Vector3 y = Vector3::cross(z, x);

			x.normalize();
			y.normalize();
			z.normalize();

			Matrix translation = CreateTranslation(-eye.x, -eye.y, -eye.z);

			Matrix rotation;
			rotation[0] = x.x; rotation[4] = x.y; rotation[8] = x.z;
			rotation[1] = y.x; rotation[5] = y.y; rotation[9] = y.z;
			rotation[2] = z.x; rotation[6] = z.y; rotation[10] = z.z;

			return rotation * translation;
		}
	}
}

#endif // ___Common_MatrixTransform_h___

