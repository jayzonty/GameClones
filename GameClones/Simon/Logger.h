#pragma once

#ifndef ___Common_Logger_h___
#define ___Common_Logger_h___

#include <string>

namespace Common {
	class Logger {
	public:
		enum class LogLevel {
			DEBUG, LOG
		};

		Logger();
		~Logger();

		virtual void LogMessage(const std::string& message, const LogLevel& logLevel) = 0;
	};
}

#endif // ___Common_Logger_h___
