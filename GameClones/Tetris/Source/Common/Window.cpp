#include "Window.h"

namespace Common {
	Window::Window() {
	}

	Window::~Window() {

	}

	void Window::SetSize(int width, int height) {
		glfwSetWindowSize(m_handle, width, height);
	}

	void Window::GetSize(int* width, int* height) const {
		glfwGetWindowSize(m_handle, width, height);
	}

	int Window::GetWidth() const {
		int ret;
		GetSize(&ret, nullptr);

		return ret;
	}

	int Window::GetHeight() const {
		int ret;
		GetSize(nullptr, &ret);

		return ret;
	}

	void Window::SetTitle(const char* title) {
		glfwSetWindowTitle(m_handle, title);
	}

	GLFWwindow* Window::GetHandle() const {
		return m_handle;
	}
}