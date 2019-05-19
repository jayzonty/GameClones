#include "BaseApplication.h"

#include "Input.h"
#include "WindowManager.h"
#include "Window.h"

#include <chrono>

namespace Common {
	BaseApplication::BaseApplication(const std::string& applicationName) : 
		m_applicationName(applicationName) {
	}

	BaseApplication::~BaseApplication() {
	}

	void BaseApplication::Run() {
		Init();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		auto prevTime = std::chrono::system_clock::now();

		Window* mainWindow = WindowManager::GetMainWindow();
		while (!glfwWindowShouldClose(mainWindow->GetHandle())) {
			int width, height;
			mainWindow->GetSize(&width, &height);
			glViewport(0, 0, width, height);

			auto currentTime = std::chrono::system_clock::now();
			std::chrono::duration<float> timeDiff = currentTime - prevTime;
			prevTime = currentTime;

			float deltaTime = timeDiff.count();
			Update(deltaTime);

			glClear(GL_COLOR_BUFFER_BIT);
			Draw();

			glfwSwapBuffers(mainWindow->GetHandle());

			Input::Update();
			glfwPollEvents();
		}
		
		Cleanup();
	}
}