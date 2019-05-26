#pragma once

#ifndef ___Common_BaseApplication_h___
#define ___Common_BaseApplication_h___

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Window.h"

namespace Common {
	class BaseApplication {
	private:
		std::string m_applicationName;

	public:
		BaseApplication(const std::string& applicationName);
		virtual ~BaseApplication();

		void Run();

	protected:
		// Override to do custom setting up of window,
		// such as setting title, setting window size
		virtual void SetupWindow(Window* window);

		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void Cleanup() = 0;

	};
}

#endif // ___Common_BaseApplication_h___
