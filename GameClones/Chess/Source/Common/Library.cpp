#include "Library.h"

#include "GLIncludes.h"

#include "Input.h"
#include "WindowManager.h"

namespace Common
{
	bool Library::Initialize()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		WindowManager::Initialize();
		glfwMakeContextCurrent(WindowManager::m_mainWindow->GetHandle());

		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
		{
			WindowManager::Cleanup();
			glfwTerminate();

			return false;
		}

		Input::Initialize();
		glfwSetMouseButtonCallback(WindowManager::m_mainWindow->GetHandle(), Input::MouseButtonCallback);
		glfwSetKeyCallback(WindowManager::m_mainWindow->GetHandle(), Input::KeyCallback);
		glfwSetCursorPosCallback(WindowManager::m_mainWindow->GetHandle(), Input::CursorCallback);
		
		return true;
	}

	void Library::Cleanup()
	{
		glfwTerminate();

		WindowManager::Cleanup();
		Input::Cleanup();
	}
}
