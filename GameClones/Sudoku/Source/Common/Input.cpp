#include "Input.h"

#include <cmath>

namespace Common
{
	Input* Input::m_instance = nullptr;

	Input::Input()
	{
	}

	Input::~Input()
	{
	}

	bool Input::IsPressed(int key)
	{
		if (nullptr == m_instance)
		{
			return false;
		}

		return (m_instance->m_pressedKeys.find(key) !=
			m_instance->m_pressedKeys.end());
	}

	bool Input::IsAnyPressed(std::initializer_list<int> keys)
	{
		for (int key : keys)
		{
			if (IsPressed(key))
			{
				return true;
			}
		}

		return false;
	}

	bool Input::IsReleased(int key)
	{
		if (nullptr == m_instance)
		{
			return false;
		}

		return (m_instance->m_releasedKeys.find(key) !=
			m_instance->m_releasedKeys.end());
	}

	bool Input::IsAnyReleased(std::initializer_list<int> keys)
	{
		for (int key : keys)
		{
			if (IsReleased(key))
			{
				return true;
			}
		}

		return false;
	}

	bool Input::IsDown(int key)
	{
		if (nullptr == m_instance)
		{
			return false;
		}

		return (m_instance->m_heldKeys.find(key) !=
			m_instance->m_heldKeys.end());
	}

	bool Input::IsAnyDown(std::initializer_list<int> keys)
	{
		for (int key : keys)
		{
			if (IsDown(key))
			{
				return true;
			}
		}

		return false;
	}

	void Input::GetMousePosition(int* mouseX, int* mouseY)
	{
		if (nullptr == m_instance)
		{
			return;
		}

		if (nullptr != mouseX)
		{
			*mouseX = m_instance->m_mousePositionX;
		}
		if (nullptr != mouseY)
		{
			*mouseY = m_instance->m_mousePositionY;
		}
	}

	int Input::GetMouseX()
	{
		if (nullptr == m_instance)
		{
			return 0;
		}

		return m_instance->m_mousePositionX;
	}

	int Input::GetMouseY()
	{
		if (nullptr == m_instance)
		{
			return 0;
		}

		return m_instance->m_mousePositionY;
	}

	void Input::Initialize()
	{
		if (nullptr == m_instance)
		{
			m_instance = new Input();
		}
	}

	void Input::Cleanup()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	void Input::Update()
	{
		if (nullptr == m_instance)
		{
			return;
		}

		m_instance->m_pressedKeys.clear();
		m_instance->m_releasedKeys.clear();
	}

	void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (nullptr == m_instance)
		{
			return;
		}

		if (action == GLFW_PRESS)
		{
			m_instance->m_pressedKeys.insert(button);
			m_instance->m_heldKeys.insert(button);
		}
		else if (action == GLFW_RELEASE)
		{
			m_instance->m_pressedKeys.erase(button);
			m_instance->m_heldKeys.erase(button);

			m_instance->m_releasedKeys.insert(button);
		}
	}

	void Input::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		if (nullptr == m_instance)
		{
			return;
		}

		if (action == GLFW_PRESS)
		{
			m_instance->m_pressedKeys.insert(key);
			m_instance->m_heldKeys.insert(key);
		}
		else if (action == GLFW_RELEASE)
		{
			m_instance->m_pressedKeys.erase(key);
			m_instance->m_heldKeys.erase(key);

			m_instance->m_releasedKeys.insert(key);
		}
	}

	void Input::CursorCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (nullptr == m_instance)
		{
			return;
		}

		m_instance->m_mousePositionX = static_cast<int>(floor(xPos));
		m_instance->m_mousePositionY = static_cast<int>(floor(yPos));
	}
}
