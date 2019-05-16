#pragma once

#ifndef ___Common_Matrix_h___
#define ___Common_Matrix_h___

#include <cstring>

namespace Common {
    class Matrix {
    public:
        float mat[16];

		Matrix() {
			memset(mat, 0, sizeof(mat));
			mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
		}

		float& operator[](int index) {
			return mat[index];
		}

		const float& operator[](int index) const {
			return mat[index];
		}

		static Matrix Identity() {
			return Matrix();
		}
    };

	inline Matrix operator*(const Matrix& a, const Matrix& b) {
		Matrix ret;
		for (int i = 0; i < 16; i++)
		{
			ret[i] = 0.0f;
			for (int j = 0; j < 4; j++)
			{
				ret[i] += a[i % 4 + j * 4] * b[i - (i % 4) + j];
			}
		}

		return ret;
	}

	inline Matrix operator*(const Matrix& a, const float& s) {
		Matrix ret;
		for (int i = 0; i < 16; i++)
		{
			ret[i] = a[i] * s;
		}

		return ret;
	}
}

#endif // ___Common_Matrix_h___
