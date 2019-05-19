#pragma once

#ifndef ___Common_WindowManager_h___
#define ___Common_WindowManager_h___

#include "Window.h"

namespace Common {
	class WindowManager {
		friend class Library;

		static Window* m_mainWindow;

		const static int DEFAULT_WINDOW_WIDTH = 800;
		const static int DEFAULT_WINDOW_HEIGHT = 600;

		const static char* DEFAULT_WINDOW_TITLE;
	public:
		static Window* GetMainWindow();

	private:
		static void Initialize();
		static void Cleanup();

	};
}

#endif // ___Common_WindowManager_h___
