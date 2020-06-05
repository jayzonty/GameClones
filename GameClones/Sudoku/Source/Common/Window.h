#ifndef ___Common_Window_h___
#define ___Common_Window_h___

#include <GLFW/glfw3.h>

namespace Common
{
	class Window
	{
		friend class WindowManager;

		GLFWwindow* m_handle;

	public:
		Window();
		~Window();

		void SetSize(int width, int height);
		void GetSize(int* width, int* height) const;
		int GetWidth() const;
		int GetHeight() const;

		void SetTitle(const char* title);

		GLFWwindow* GetHandle() const;
	};
}

#endif // ___Common_Window_h___