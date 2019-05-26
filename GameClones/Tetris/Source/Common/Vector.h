#pragma once

#ifndef ___Common_Vector_h___
#define ___Common_Vector_h___

#include <cmath>

namespace Common {
    class Vector2 {
    public:
        float x, y;

		Vector2(float newX, float newY) : x(newX), y(newY) {
		}

		Vector2(float val) : Vector2(val, val) {
		}

		Vector2() : Vector2(0.0f) {
		}

		Vector2(const Vector2& other) {
			x = other.x;
			y = other.y;
		}

		static float Dot(const Vector2& a, const Vector2& b) {
			return a.x * b.x + a.y * b.y;
		}
        
		float SquareMagnitude() const {
			return x * x + y * y;
		}

		float Magnitude() const {
			float squareMag = SquareMagnitude();
			return sqrt(squareMag);
		}

		void Normalize() {
			if (x != 0.0f || y != 0.0f)
			{
				float mag = Magnitude();
				x /= mag;
				y /= mag;
			}
		}

		Vector2 GetNormalized() const {
			Vector2 v(x, y);
			v.Normalize();

			return v;
		}

        // Operators
		Vector2& operator+=(const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Vector2& operator-=(const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		Vector2& operator*=(const float& rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		Vector2& operator/=(const float& rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		Vector2 operator-() {
			Vector2 ret(-x, -y);
			return ret;
		}

		Vector2& operator=(const Vector2& other) {
			x = other.x;
			y = other.y;
			return *this;
		}
    };

	static Vector2 operator+(Vector2 a, const Vector2& b) {
		a += b;
		return a;
	}

	static Vector2 operator-(Vector2 a, const Vector2& b) {
		a -= b;
		return a;
	}

	static Vector2 operator*(Vector2 v, const float& s) {
		v *= s;
		return v;
	}

	static Vector2 operator/(Vector2 v, const float& s) {
		v /= s;
		return v;
	}

    class Vector3 {
    public:
        float x, y, z;

		Vector3(float newX, float newY, float newZ) : 
			x(newX), y(newY), z(newZ) {
		}

		Vector3(float val) : Vector3(val, val, val) {
		}

		Vector3() : Vector3(0.0f) {
		}

		Vector3(const Vector3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
		}
		
		static float Dot(const Vector3& a, const Vector3& b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		static Vector3 Cross(const Vector3& a, const Vector3& b) {
			Vector3 ret;
			ret.x = a.y * b.z - a.z * b.y;
			ret.y = a.z * b.x - a.x * b.z;
			ret.z = a.x * b.y - a.y * b.x;

			return ret;
		}

		float SquareMagnitude() const {
			return x * x + y * y + z * z;
		}

		float Magnitude() const {
			float squareMag = SquareMagnitude();
			return sqrt(squareMag);
		}

		void Normalize() {
			if (x != 0.0f || y != 0.0f || z != 0.0f)
			{
				float mag = Magnitude();
				x /= mag;
				y /= mag;
				z /= mag;
			}
		}

		Vector3 GetNormalized() const {
			Vector3 v(x, y, z);
			v.Normalize();

			return v;
		}

		Vector3& operator+=(const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}

		Vector3& operator-=(const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		Vector3& operator*=(const float& rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;

			return *this;
		}

		Vector3& operator/=(const float& rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

		Vector3 operator-() {
			Vector3 ret(-x, -y, -z);
			return ret;
		}

		Vector3& operator=(const Vector3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}
    };

	static Vector3 operator+(Vector3 a, const Vector3& b) {
		a += b;
		return a;
	}

	static Vector3 operator-(Vector3 a, const Vector3& b) {
		a -= b;
		return a;
	}

	static Vector3 operator*(Vector3 a, const float& s) {
		a *= s;
		return a;
	}

	static Vector3 operator/(Vector3 a, const float& s) {
		a /= s;
		return a;
	}
}

#endif // ___Common_Vector_h___

