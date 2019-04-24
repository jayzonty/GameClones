#pragma once

#ifndef ___GameClones_Vector_h___
#define ___GameClones_Vector_h___

#include <cmath>

namespace GameClones {
	struct Vector2f {
		float x;
		float y;

		Vector2f(float newX, float newY) : x(newX), y(newY) {}
		Vector2f() : Vector2f(0.0f, 0.0f) {}

		Vector2f operator+(const Vector2f& rhs) const {
			return Vector2f(x + rhs.x, y + rhs.y);
		}

		Vector2f& operator+=(const Vector2f& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Vector2f& operator=(const Vector2f& rhs) {
			x = rhs.x;
			y = rhs.y;

			return *this;
		}

		Vector2f& operator*=(const float& rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		Vector2f operator/(const float& rhs) {
			return Vector2f(x / rhs, y / rhs);
		}

		Vector2f& operator/=(const float& rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		float SqrMagnitude() {
			return x * x + y * y;
		}

		float Magnitude() {
			return sqrtf(SqrMagnitude());
		}

		float Normalize() {
			float magnitude = Magnitude();

			x /= magnitude;
			y /= magnitude;
		}

		friend Vector2f operator*(const Vector2f& lhs, const float& rhs);
	};

	Vector2f operator*(const Vector2f& lhs, const float& rhs) {
		return Vector2f(lhs.x * rhs, lhs.y * rhs);
	}
}

#endif // ___GameClones_Vector_h___
