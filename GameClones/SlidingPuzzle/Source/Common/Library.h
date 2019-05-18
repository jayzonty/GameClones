#pragma once

#ifndef ___Common_Library_h___
#define ___Common_Library_h___

namespace Common {
	class Library {
	public:
		static bool Initialize();
		static void Cleanup();
	};
}

#endif // ___Common_Library_h___
