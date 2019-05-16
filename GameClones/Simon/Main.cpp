#include <iostream>
#include <cstddef>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>

#include "Game.h"

#include <crtdbg.h>

GameClones::Simon::Game* game = nullptr;

void HandleMouseInput(GLFWwindow* window, int button, int action, int mods) {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	game->HandleMouseInput(button, action, mods, (float)mouseX, (float)mouseY);
}

int main() {
	_CrtMemState s1, s2, s3;

	_CrtMemCheckpoint(&s1);

	{
		if (!glfwInit()) {
			return 1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(800, 600, "Simon", nullptr, nullptr);
		if (nullptr == window) {
			return false;
		}
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			return 1;
		}

		glfwSwapInterval(1);

		glfwSetMouseButtonCallback(window, HandleMouseInput);

		game = new GameClones::Simon::Game(800, 600, window);
		game->Init();

		auto prevTime = std::chrono::system_clock::now();
		while (!glfwWindowShouldClose(window)) {
			std::chrono::duration<float> deltaTime = std::chrono::system_clock::now() - prevTime;
			prevTime = std::chrono::system_clock::now();

			glClear(GL_COLOR_BUFFER_BIT);

			game->Update(deltaTime.count());
			game->Draw();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glfwTerminate();

		delete game;
	}

	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2)) {
		std::printf("Memory leak!\n");
	}

	return 0;
}
