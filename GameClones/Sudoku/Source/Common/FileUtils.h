#ifndef ___Common_FileUtils_h___
#define ___Common_FileUtils_h___

#include <string>
#include <vector>

namespace Common
{
	namespace FileUtils
	{
		extern bool ReadFile(const char* filePath, std::string& outputString);

		extern bool ReadLinesFromFile(const char* filePath, std::vector<std::string>& outLines);
	}
}

#endif // ___Common_FileUtils_h___
