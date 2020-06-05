#ifndef ___Common_Input_h___
#define ___Common_Input_h___

#include <GLFW/glfw3.h>

#include <unordered_set>

namespace Common
{
	class Input
	{
	public:
		enum Key
		{
			UP = GLFW_KEY_UP,
			DOWN = GLFW_KEY_DOWN,
			LEFT = GLFW_KEY_LEFT,
			RIGHT = GLFW_KEY_RIGHT,

			SPACE = GLFW_KEY_SPACE,

			R = GLFW_KEY_R,
			S = GLFW_KEY_S,
			
			ESC = GLFW_KEY_ESCAPE,

			ALPHA0 = GLFW_KEY_0,
			ALPHA1 = GLFW_KEY_1,
			ALPHA2 = GLFW_KEY_2,
			ALPHA3 = GLFW_KEY_3,
			ALPHA4 = GLFW_KEY_4,
			ALPHA5 = GLFW_KEY_5,
			ALPHA6 = GLFW_KEY_6,
			ALPHA7 = GLFW_KEY_7,
			ALPHA8 = GLFW_KEY_8,
			ALPHA9 = GLFW_KEY_9
		};

		enum Button
		{
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
