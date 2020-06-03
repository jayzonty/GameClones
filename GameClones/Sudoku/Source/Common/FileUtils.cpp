#include "FileUtils.h"

#include <fstream>
#include <cstdio>

namespace Common {
	namespace FileUtils {
		bool ReadFile(const char* filePath, std::string& outputString) {
			std::ifstream file(filePath);
			if (file.fail()) {
				return false;
			}

			std::string line;
			while (std::getline(file, line)) {	
				outputString += line + "\n";
			}

			return true;
		}
	}
}