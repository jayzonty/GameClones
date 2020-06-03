#pragma once

#ifndef ___Common_MemoryLeakTester_h___
#define ___Common_MemoryLeakTester_h___

#if defined(_WIN32) || defined(_WIN64)
#include <crtdbg.h>
#endif

#include <cstdio>

namespace Common {
	class MemoryLeakTester {
#if defined(_WIN32) || defined(_WIN64)
		_CrtMemState s1, s2, s3;
#endif
	public:
		MemoryLeakTester(void) = default;
		~MemoryLeakTester(void) = default;

		void Start() {
#if defined(_WIN32) || defined(_WIN64)
			_CrtMemCheckpoint(&s1);
#endif
		}

		// Returns true if there are uncleared memory
		bool End() {
			bool ret = false;

#if defined(_WIN32) || defined(_WIN64)
			_CrtMemCheckpoint(&s2);
			if (_CrtMemDifference(&s3, &s1, &s2)) {
				ret = true;
			}
#endif

			if (ret) {
				std::printf("Memory leak!\n");
			}

			return ret;
		}
	};
}

#endif // ___Common_MemoryLeakTester_h___
