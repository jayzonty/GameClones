#pragma once

#ifndef ___Common_BaseApplication_h___
#define ___Common_BaseApplication_h___

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace Common {
	class BaseApplication {
	private:
		std::string m_applicationName;

	public:
		BaseApplication(const std::string& applicationName);
		virtual ~BaseApplication();

		void Run();

	protected:
		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void Cleanup() = 0;

	};
}

#endif // ___Common_BaseApplication_h___
