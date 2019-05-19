#pragma once

#ifndef ___Common_FileUtils_h___
#define ___Common_FileUtils_h___

#include <string>

namespace Common {
	namespace FileUtils {
		bool ReadFile(const char* filePath, std::string& outputString);
	}
}

#endif // ___Common_FileUtils_h___
