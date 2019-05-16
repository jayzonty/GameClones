#pragma once

#ifndef ___Common_MathUtils_h___
#define ___Common_MathUtils_h___

namespace Common {
	namespace MathUtils {
		const double PI_D = atan(1.0) * 4.0;
		const float PI_F = atanf(1.0) * 4.0f;

		template <class T>
		T Clamp(T value, T min, T max) {
			if (value < min) {
				value = min;
			}
			else if (value > max) {
				value = max;
			}

			return value;
		}

		template <class T>
		T Min(T a, T b) {
			if (a <= b) {
				return a;
			}
			return b;
		}

		template <class T>
		T Max(T a, T b) {
			if (a > b) {
				return a;
			}
			return b;
		}
	}
}

#endif // ___Common_MathUtils_h___
