#pragma once

#ifndef ___Common_Input_h___
#define ___Common_Input_h___

#include <GLFW/glfw3.h>

#include <unordered_set>

namespace Common {
	class Input {
	public:
		enum Key {
			UP = GLFW_KEY_UP,
			DOWN = GLFW_KEY_DOWN,
			LEFT = GLFW_KEY_LEFT,
			RIGHT = GLFW_KEY_RIGHT,

			SPACE = GLFW_KEY_SPACE,

			R = GLFW_KEY_R,
			S = GLFW_KEY_S,
			
			ESC = GLFW_KEY_ESCAPE
		};

		enum Button {
			LEFT_MOUSE = GLFW_MOUSE_BUTTON_LEFT,
			MIDDLE_MOUSE = GLFW_MOUSE_BUTTON_MIDDLE,
			RIGHT_MOUSE = GLFW_MOUSE_BUTTON_RIGHT
		};

	private:
		friend class BaseApplication;
		friend class Library;

		static Input* m_instance;

		std::unordered_set<int> m_pressedKeys;
		std::unordered_set<int> m_releasedKeys;

		std::unordered_set<int> m_heldKeys;

		int m_mousePositionX, m_mousePositionY;

		Input();

	public:
		~Input();

		// Is the key/button just pressed during this frame?
		static bool IsPressed(int key);

		// Is the key/button just released in this frame?
		static bool IsReleased(int key);

		// Is the key/button being pressed/held down?
		static bool IsDown(int key);

		static void GetMousePosition(int* mouseX, int* mouseY);

		static int GetMouseX();

		static int GetMouseY();
		
	private:
		static void Initialize();
		static void Cleanup();

		static void Update();

		static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorCallback(GLFWwindow* window, double xPos, double yPos);
	};
}

#endif // ___Common_Input_h___
