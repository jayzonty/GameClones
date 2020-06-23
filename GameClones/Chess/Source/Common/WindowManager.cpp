#include "WindowManager.h"

#include <GLFW/glfw3.h>

namespace Common
{
	Window* WindowManager::m_mainWindow = nullptr;
	const char* WindowManager::DEFAULT_WINDOW_TITLE = "Untitled";

	Window* WindowManager::GetMainWindow()
	{
		return m_mainWindow;
	}

	void WindowManager::Initialize()
	{
		m_mainWindow = new Window();
		m_mainWindow->m_handle = glfwCreateWindow(
			DEFAULT_WINDOW_WIDTH,
			DEFAULT_WINDOW_HEIGHT,
			DEFAULT_WINDOW_TITLE, nullptr, nullptr);
	}

	void WindowManager::Cleanup()
	{
		if (nullptr != m_mainWindow)
		{
			glfwDestroyWindow(m_mainWindow->GetHandle());
		}

		delete m_mainWindow;
	}
}