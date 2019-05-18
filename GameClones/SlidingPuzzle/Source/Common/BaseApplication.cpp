#include "BaseApplication.h"

#include "Input.h"
#include "WindowManager.h"
#include "Window.h"

namespace Common {
	BaseApplication::BaseApplication(const std::string& applicationName) : 
		m_applicationName(applicationName) {
	}

	BaseApplication::~BaseApplication() {
	}

	void BaseApplication::Run() {
		Init();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Window* mainWindow = WindowManager::GetMainWindow();
		while (!glfwWindowShouldClose(mainWindow->GetHandle())) {
			glClear(GL_COLOR_BUFFER_BIT);

			int width, height;
			mainWindow->GetSize(&width, &height);
			glViewport(0, 0, width, height);

			float deltaTime = 0.0f;
			Update(deltaTime);
			Draw();

			glfwSwapBuffers(mainWindow->GetHandle());

			Input::Update();
			glfwPollEvents();
		}
		
		Cleanup();
	}
}